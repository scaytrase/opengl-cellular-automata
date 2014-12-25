#include "glmushroom_model.h"
#include <QFile>
#include <QList>
#include <qmath.h>

/*!
  Инициализируется логгером. Теоретически можно использовать свой логгер для модели.
*/
glmushroom_model::glmushroom_model(glmushroom_logger *main_logger /*!< указатель на логгер модели */)
{
	logger = main_logger;
	inited = false;
	inited = init();
}

bool glmushroom_model::init(){
	QFile input_data("forest.txt");
	if (!input_data.open(QIODevice::ReadOnly)){
		*logger << "Cannot open 'forest.txt'";
		return false;
	}
	int sizeX=0,sizeY=0;
	QByteArray input_buffer;
	main_field.clear();
	QVector<float> field_row;
	input_buffer = input_data.readLine ();
	while ( !input_buffer.isEmpty ()){
		sizeX = input_buffer.length() - 1;
		for (int i = 0; i < sizeX; i++){
			bool is_tree = (input_buffer.at (i) > '0');
			field_row.append (is_tree?1:0);
		}
		main_field.append (field_row);
		field_row.clear ();
		sizeY++;
		input_buffer = input_data.readLine ();
	}
	field_size_x = sizeX;
	field_size_y = sizeY;
	if (!((sizeX > 1) && (sizeY > 1))) {
		*logger << "Invalid data file";
		return false;
	}
	return true;
}

/*!
  Функция для чтения field_size_x;
*/
int glmushroom_model::sizeX () const{
	return field_size_x;
}

/*!
  Функция для чтения field_size_y;
*/
int glmushroom_model::sizeY () const{
	return field_size_y;
}

/*!
  Функция для чтения данных модели;
*/

QVector< QVector <float> > glmushroom_model::get_data () const{
	return main_field;

}

/*!
  Функция выполняющая расчет следующего поколения экосистемы, основываясь на текущем состоянии.
*/
QVector< QVector <float> > glmushroom_model::next_age() {

	QVector< QVector <float> > result = main_field;
	for (int x = 0; x < field_size_x; x++)
		for (int y = 0; y < field_size_y; y++){
		QList<int> free_list;
		QList<int> used_list;
		QList<int> tree_list;
		free_list.clear();
		used_list.clear();
		tree_list.clear();
		int idx = 0;
		for (int fy = -1; fy < 2; fy ++)
			for (int fx = -1; fx < 2; fx ++){

			int ax = (field_size_x + x + fx ) % field_size_x;
			int ay = (field_size_y + y + fy ) % field_size_y;

			//if (main_field[ay][ax] < -5) main_field[ay][ax] = 0; // метим мёртвое пустым

			if ((main_field
				 [ay][ax] >= -1) &&
				(main_field
				 [ay][ax]<= 0))
				free_list.append(idx);
			if ((main_field
				 [ay][ax]) < -1)
				used_list.append(idx);
			if ((main_field
				 [ay][ax]) > 0)
				tree_list.append(idx);
			idx++;
		}
		//Mushroom case
		if (main_field[y][x] < -1)
		{
			int new_mush_number = myrand(3);

			for (int i = 0; i < new_mush_number; i++){
				if (free_list.length() > 0){
					int new_mush_idx = myrand(free_list.length());
					int new_mush_id = free_list.at(new_mush_idx);
					free_list.removeAt(new_mush_idx);
					int cur_id = 0;
					for (int fy = -1; fy < 2; fy ++)
						for (int fx = -1; fx < 2; fx ++){
						if(cur_id == new_mush_id){
							result
									[(field_size_y + y + fy ) % field_size_y]
									[(field_size_x + x + fx ) % field_size_x]
									= -6;
						}
						cur_id++;
					}
				}
			}//end of growing new mushrooms around old one
			//result[y][x] = main_field[y][x] + 1;
			//result[y][x] = round(main_field[y][x]);

			if (used_list.length() > 2)
				result[y][x] /= 10.f; // умирающее
		}
		if (main_field[y][x] > 0) {//if tree

			if (free_list.length() > 0 ) {

				int new_mush_idx = myrand( free_list.length() + 1);
				if (new_mush_idx != free_list.length()) {
					int new_mush_id = free_list.at(new_mush_idx);
					free_list.removeAt(new_mush_idx);
					int cur_id = 0;
					for (int fy = -1; fy < 2; fy ++){
						for (int fx = -1; fx < 2; fx ++){
							if(cur_id == new_mush_id){
								result
										[(field_size_y + y + fy ) % field_size_y]
										[(field_size_x + x + fx ) % field_size_x]
										= -6;
							}
							cur_id++;
						}
					}
				}
			}
			result[y][x]=1;
		}// end of growing new mushroom around tree

	}//end of for by cells
	return result;
}//end of next_age

/*!
  Функция для управления моделью методом отсылки сообщений событий
*/

void glmushroom_model::send_Event(model_Event event /*!<  Событие, передаваемое модели*/){
	switch (event){
	case EVENT_TOUCH: return;
	case EVENT_TICK: main_field = next_age(); return;
	case EVENT_SUBTICK: main_field = next_subage(); return;
	case EVENT_RELOAD: init(); return;
	}
}

/*!
  Возвращает случайное число по модулю count
*/
int glmushroom_model::myrand(int count/*!<  Ограничитель случайного числа*/){
	//qsrand(clock() % 241);
	long newrand = qAbs(qrand() * qrand() * qrand()) % 117234;
	return newrand % count;
}


/*!
  Функция выполняющая устаревание грибов
*/
QVector< QVector <float> > glmushroom_model::next_subage() {
	QVector< QVector <float> > result = main_field;

	for (int x = 0; x < field_size_x; x++)
		for (int y = 0; y < field_size_y; y++){
			//if (result[y][x] < -7) result[y][x] = 0;
			//else if (result[y][x] < -5) result[y][x] -= 3*MUSHROOM_SUBSTEP;
			if (result[y][x] < 0) result[y][x] += MUSHROOM_SUBSTEP;
			if (result[y][x] > 0 && result[y][x] < 1) result[y][x] = 0;
			//else if (result[y][x] < 1) result[y][x] = 0;
		}

	return result;
}
