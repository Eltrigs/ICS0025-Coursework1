#include "Data.h"
#include <iostream>

Data::Data(int n)
{
	//Start adding n objects of type Item.
	for (int i = 0; i < n; i++)
	{
		//Create a new random item using it's constructor
		Item item = Item();
		//Insert it like any other Item object
		//std::cout << item.getGroup() << "\t" << item.getSubgroup() << "\t" << item.getName() << "\t" << item.getTimestamp()<< std::endl;
		this->InsertItem(item.getGroup(), item.getSubgroup(), item.getName(), item.getTimestamp());
		item.~Item();
	}
}

Data::Data()
{
}

Data::~Data()
{
}

void Data::PrintAll()
{
	//Loop through all groups and print them
	auto print = [](const Item item) { std::cout << item.getName() + " " + item.getTimestamp().ToString() << std::endl; };

	//Iterate through all groups
	for (auto groupIterator = DataStructure.cbegin(); groupIterator != DataStructure.cend(); ++groupIterator) {
		auto group = groupIterator->second;
		std::cout << groupIterator->first << ':' << std::endl; // print group key

		//Iterate through all subgroups
		for (auto subgroupIterator = group->cbegin(); subgroupIterator != group->cend(); ++subgroupIterator) {
			std::cout << " " << subgroupIterator->first << ':' << std::endl; // print subgroup key

			// Print every item in the subrgoup using the print lambda
			std::for_each(subgroupIterator->second->cbegin(), subgroupIterator->second->cend(), print);
		}

		std::cout << std::endl << std::endl;
	}
}

int Data::CountItems()
{
	return 0;
}

std::map<int, std::set<Item>*>* Data::GetGroup(char c)
{
	auto groupIterator = DataStructure.find(c);
	if (groupIterator == DataStructure.end()) //The group does not exist
	{
		return nullptr;
	}
	else //The group exists, return it
	{
		return groupIterator->second;
	}
}

void Data::PrintGroup(char c)
{
	//Find group, then
	//Loop through all subgroups and print them
}

int Data::CountGroupItems(char c)
{
	return 0;
}

std::set<Item>* Data::GetSubgroup(char c, int i)
{
	auto group = GetGroup(c);
	if (group != nullptr)
	{
		auto subgroupIterator = group->find(i);
		if (subgroupIterator == group->end()) //The subgroup does not exist
		{
			return nullptr;
		}
		else //The subgroup exists, return it
		{
			return subgroupIterator->second;
		}
	}
	return nullptr;
}

void Data::PrintSubgroup(char c, int i)
{
	//Find subgroup, then
	//Loop through all subgroups, printing them
}

int Data::CountSubgroupItems(char c, int i)
{
	return 0;
}

std::optional<Item> Data::GetItem(char c, int i, std::string s)
{
	auto subgroup = GetSubgroup(c, i);
	if (subgroup != nullptr)
	{
		Item tempItem = Item(c, i, s, Date());
		for (const auto& item : *subgroup) {
		}
		auto itemIterator = subgroup->find(tempItem);
		if (itemIterator == subgroup->end()) //The Item does not exist
		{
			return std::nullopt;
		}
		else //The Item exists, return it
		{
			return *itemIterator;
		}
	}
	return std::nullopt;
}

void Data::PrintItem(char c, int i, std::string s)
{
	//Find item, then print it's timestamp?
}

std::optional<Item> Data::InsertItem(char c, int i, std::string s, std::optional<Date> d)
{
	//Check for empty string
	std::optional<Item> testItem = GetItem(c, i, s);
	if (s.empty() || testItem != std::nullopt)
	{
		return std::nullopt;
	}

	//Create the new item with the given function parameters
	Item newItem = Item(c, i, s, d.value_or(Date::CreateRandomDate(Item::Begin, Item::End)));

	//First check if the group already exists
	auto groupIterator = DataStructure.find(c);

	if (groupIterator == DataStructure.end()) // The group doesn't exist
	{
		//Insert a new group using data from the newItem object
		InsertGroup(newItem.getGroup(), { newItem.getSubgroup() }, { {std::tuple<std::string, std::optional<Date>>(newItem.getName(), newItem.getTimestamp())} });
		return newItem;
	}
	else //The group exists
	{
		//Check if the Subgroup exists for newItem
		auto currentGroup = groupIterator->second;
		auto subGroupIterator = currentGroup->find(i);

		if (subGroupIterator == currentGroup->end()) //Subgroup doesn't exist
		{
			//Create a new subgroup
			InsertSubgroup(newItem.getGroup(), newItem.getSubgroup(), { std::tuple<std::string, std::optional<Date>>(newItem.getName(), newItem.getTimestamp()) });
			return newItem;
		}
		else //The subgroup exists
		{
			//Check if the exact same Item object exists
			auto currentSubgroup = subGroupIterator->second;
			auto itemIterator = subGroupIterator->second->find(newItem);
			if (itemIterator == currentSubgroup->end()) // Item does not exist
			{
				currentSubgroup->insert(newItem);
				return newItem;
			}
			else //An exact match was found
			{
				return std::nullopt;
			}
		}
	}
}

std::set<Item>* Data::InsertSubgroup(char c, int i, std::initializer_list<std::tuple<std::string, std::optional<Date>>> items)
{
		//Check if the subgroup exists and if inputs are correct?
	if (GetSubgroup(c, i))
	{
		return nullptr;
	}

	//Find the group that the subgroup is to be inserted into
	auto groupIterator = DataStructure.find(c);
	if (groupIterator == DataStructure.end()) // The Group itself does not exist
	{
		InsertGroup(c, { i }, { items });
		//Now that the group has been added, to return from this code find this new subgroup and return it
		groupIterator = DataStructure.find(c);
		auto subgroupIterator = groupIterator->second;
		auto subgroup = subgroupIterator->find(i)->second;
		return subgroup;
	}
	else //The Group exists, subgroup doesn't
	{
		auto currentGroupValue = groupIterator->second;
		//Create the std::set that is needed for a new subgroup:
		std::set<Item> *subGroupSet = new std::set<Item>();
		//Add a new subgroup to the map with key i and value subGroupSet
		currentGroupValue->insert({ i, subGroupSet });
		/*
		* Alternatives for this same operation:
		* currentGroupValue->emplace(i, subGroupSet);
		* currentGroupValue->insert({ i, &subGroupSet });
		* currentGroupValue->insert(std::make_pair(i, &subGroupSet));
		*/

		//Insert all the items into the newly emplaced empty subGroup
		for (std::tuple<std::string, std::optional<Date>> tuple : items)
		{
			InsertItem(c, i, std::get<0>(tuple), std::get<1>(tuple));
		}
		return DataStructure.find(c)->second->find(i)->second;
	}
}

std::map<int, std::set<Item>*>* Data::InsertGroup(char c, std::initializer_list<int> subgroups, std::initializer_list<std::initializer_list<std::tuple<std::string, std::optional<Date>>>> items)
{
	//Check if the group exists and if inputs are correct?
	if (GetGroup(c) || subgroups.size() != items.size())
	{
		return nullptr;
	}

	//Create the new group where to fit all of the data
	std::map<int, std::set<Item>*>* newGroup = new std::map<int, std::set<Item>*>();

	//Insert this new empty group into the DataStructure
	DataStructure.emplace(c, newGroup);

	//Insert all subgroups into this new group by iterating through them at the same time
	auto subGroupIterator = subgroups.begin();
	auto itemilIterator = items.begin();
	for (subGroupIterator; subGroupIterator != subgroups.end(); subGroupIterator++, itemilIterator++)
	{
		InsertSubgroup(c, *subGroupIterator, *itemilIterator);
	}
	return DataStructure.find(c)->second;
	return nullptr;
}

bool Data::RemoveItem(char c, int i, std::string s)
{
	return false;
}

bool Data::RemoveSubgroup(char c, int i)
{
	return false;
}

bool Data::RemoveGroup(char c)
{
	return false;
}
