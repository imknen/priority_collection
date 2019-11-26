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

class InfoData {
public:
		list<List_iterator>& contain_pos = nullptr;
		List_iterator position;
		int priority;

		InfoData& operator=(InfoData &) = default;
	};
  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object)
  {

		auto it_obj = data.insert(data.end(), move(object));	
		auto& ref_tmp_list = priority_to_data[0];
		auto it_for_data = ref_tmp_list.insert(ref_tmp_list.end(), it_obj);

		id_to_position[&data.back()] = {ref_tmp_list, it_obj, 0};

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
  void Promote(Id id);

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
		data.erase(info_data.position);

		(*it).second.pop_back();

		id_to_position.erase(&ret.first);

		return ret;
	}

private:
  // Приватные поля и методы
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
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
