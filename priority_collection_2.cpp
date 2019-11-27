#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <deque>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = T*;
	using List_iterator = typename list<T>::iterator;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object)
  {

		auto it_obj = data.insert(data.end(), move(object));	
		auto& ref_tmp_list = priority_to_data[0];
		auto it_position = ref_tmp_list.insert(ref_tmp_list.end(), it_obj);

		id_to_position[&data.back()] = {&ref_tmp_list, it_position, it_obj, 0};

		return &data.back();
	}

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin)
	{

	}

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const
	{
	
	}

  // Получить объект по идентификатору
  const T& Get(Id id) const
	{
		return id;
	}

  // Увеличить приоритет объекта на 1
  void Promote(Id id)
	{
		auto current_info = id_to_position[id];
		auto& target_list = priority_to_data[current_info.priority+1];
		auto new_position = target_list.insert(target_list.end(), current_info.it_for_data);

		InfoData new_info{&target_list, new_position, current_info.it_for_data, ++current_info.priority};

		current_info.contain_pos->erase(current_info.position);

		id_to_position[id] = new_info;
	}

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const
	{
		auto it = priority_to_data.rbegin();

		return make_pair(&(*it.second.back()), *it.first);	
	}

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax()
	{
		auto it = make_move_iterator(priority_to_data.rbegin());
		pair<T, int> ret = make_pair(move(*(*it).second.back()), (*it).first);	

		auto info_data = id_to_position[&ret.first];
		data.erase(info_data.it_for_data);

		(*it).second.pop_back();

		id_to_position.erase(&ret.first);

		return ret;
	}

private:
  // Приватные поля и методы
class InfoData {
public:
		list<List_iterator>* contain_pos;
		typename list<List_iterator>::iterator position;
		List_iterator it_for_data;
		int priority;
};
  list<T> data;
  map<int, list<List_iterator>> priority_to_data;
	map<Id, InfoData> id_to_position;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  /*RUN_TEST(tr,*/ TestNoCopy();//);
  return 0;
}
