// DO NOT INCLUDE ANY OTHER LIBRARIES/FILES
#include "pointer.h"

// Compares the price of obj1 with obj2
// Returns a negative number if the price of obj1 is less than the price of obj2
// Returns a positive number if the price of obj1 is greater than the price of obj2
// Returns 0 if the price of obj1 is equal to the price of obj2
int compare_by_price(Object* obj1, Object* obj2)
{
    // IMPLEMENT THIS
    double obj1_price, obj2_price;

    obj1_price = object_price(obj1);
    obj2_price = object_price(obj2);

	if(obj1_price < obj2_price)
		return -1; 
	else if(obj1_price > obj2_price)
		return 1;
	else
		return 0;
}

// Compares the quantity of obj1 with obj2
// Returns a negative number if the quantity of obj1 is less than the quantity of obj2
// Returns a positive number if the quantity of obj1 is greater than the quantity of obj2
// Returns 0 if the quantity of obj1 is equal to the quantity of obj2
int compare_by_quantity(Object* obj1, Object* obj2)
{
    // IMPLEMENT THIS
    double obj1_quantity, obj2_quantity;
    
    obj1_quantity = object_quantity(obj1);
    obj2_quantity = object_quantity(obj2);

    if(obj1_quantity < obj2_quantity)
    	return -1;
	else if (obj1_quantity > obj2_quantity)
		return 1;
	else
		return 0;
}

// Initializes a StaticPriceObject with the given quantity, name, and price
void static_price_object_construct(StaticPriceObject* obj, unsigned int quantity, const char* name, double price)
{
    // IMPLEMENT THIS
    obj->obj.quantity = quantity;
    obj->obj.name = name;
    obj->price = price;
    obj->obj.virtual_func_table.bulk_price =(bulk_price_fn)static_bulk_price;
    obj->obj.virtual_func_table.price = (price_fn)static_price;
}

// Initializes a DynamicPriceObject with the given quantity, name, base price, and price scaling factor
void dynamic_price_object_construct(DynamicPriceObject* obj, unsigned int quantity, const char* name, double base, double factor)
{
    // IMPLEMENT THIS
    obj->base = base;
    obj->factor = factor;
    obj->obj.quantity = quantity;
    obj->obj.name = name;
    obj->obj.virtual_func_table.price = (price_fn)dynamic_price;
	obj->obj.virtual_func_table.bulk_price = (bulk_price_fn)dynamic_bulk_price;
}

// Returns the price of a StaticPriceObject or ERR_OUT_OF_STOCK if it is out of stock
double static_price(StaticPriceObject* obj)
{
    // IMPLEMENT THIS
    if(obj->obj.quantity == 0 || obj->obj.quantity < 0)
    	return ERR_OUT_OF_STOCK;

    return obj->price;
}

// Returns the price of a DynamicPriceObject or ERR_OUT_OF_STOCK if it is out of stock
// The dynamic price is calculated as the base price multiplied by (the quantity raised to the power of the scaling factor)
double dynamic_price(DynamicPriceObject* obj)
{
    // IMPLEMENT THIS
    double price;
    if(obj->obj.quantity == 0)
    	return ERR_OUT_OF_STOCK;
    
    price = (pow((obj->obj.quantity),(obj->factor)))*(obj->base);
    return price;  
}

// Returns the bulk price of purchasing multiple (indicated by quantity parameter) StaticPriceObject at a discount where the first item is regular price and the additional items are scaled by the BULK_DISCOUNT factor
// Return ERR_OUT_OF_STOCK of there is insufficient quantity available
double static_bulk_price(StaticPriceObject* obj, unsigned int quantity)
{
    // IMPLEMENT THIS
    double obj_price, bulk;
    double total = 0;

    if(obj->obj.quantity < quantity)
    	return ERR_OUT_OF_STOCK;
	else if(quantity == 0)
	{
		return 0;
	}
	else if(quantity == 1)
	{
		obj_price = object_price((Object*)obj);
		return obj_price;
	}
	else
	{
		obj_price = object_price((Object*)obj);
		quantity--;
		bulk = obj_price*BULK_DISCOUNT*quantity;
		total = obj_price + bulk;
		return total;
	}
}

// Returns the bulk price of purchasing multiple (indicated by quantity parameter) DynamicPriceObject at a discount where the first item is regular price and the additional items are scaled by the BULK_DISCOUNT factor
// This uses the same dynamic price equation from the dynamic_price function, and note that the price changes for each item that is bought
// For example, if 3 items are requested, each of them will have a different price, and this function calculates the total price of all 3 items
// Return ERR_OUT_OF_STOCK of there is insufficient quantity available
double dynamic_bulk_price(DynamicPriceObject* obj, unsigned int quantity)
{
    // IMPLEMENT THIS
    double obj1_price, bulk_price;
    double total = 0;

    if(obj->obj.quantity < quantity)
    	return ERR_OUT_OF_STOCK;
	else if(quantity == 0)
		return 0;
	else
	{
	    obj1_price = object_price((Object*)obj);
	    quantity--;

		while (quantity != 0)
		{
			bulk_price = (pow((quantity),(obj->factor)))*(obj->base)*BULK_DISCOUNT;
			total += bulk_price;
			quantity--;
		}

		total += obj1_price;
		return total;
	}
}

//
// Iterator functions
//

// Initializes an iterator to the beginning of a list
void iterator_begin(LinkedListIterator* iter, LinkedListNode** head)
{
    // IMPLEMENT THIS
    iter->curr = *head;
    iter->prev_next = head;
}

// Updates an iterator to move to the next element in the list if possible
void iterator_next(LinkedListIterator* iter)
{
    // IMPLEMENT THIS
    if(iter->curr == NULL)
    	return;
	else
	{
		iter->prev_next = &iter->curr->next;
		iter->curr = iter->curr->next;
		//iter->prev_next = &iter->curr;
	}
}

// Returns true if iterator is at the end of the list or false otherwise
bool iterator_at_end(LinkedListIterator* iter)
{
    // IMPLEMENT THIS
    if (iter->curr == NULL)
    	return true;
	else
		return false;
}

// Returns the current object that the iterator references or NULL if the iterator is at the end of the list
Object* iterator_get_object(LinkedListIterator* iter){
    // IMPLEMENT THIS
    if(iter->curr == NULL)
    	return NULL;
	else
		return iter->curr->obj;
}

// Removes the current node referenced by the iterator
// The iterator is valid after call and references the next object
// Returns removed node
LinkedListNode* iterator_remove(LinkedListIterator* iter)
{
    //IMPLEMENT THIS
    LinkedListNode* node;
    node = iter->curr;

    iter->curr = iter->curr->next;
    *(iter->prev_next) = iter->curr;

    return node;
}

// Inserts node after the current node referenced by the iterator
// The iterator is valid after call and references the same object as before
// Returns ERR_INSERT_AFTER_END error if iterator at the end of the list or 0 otherwise
int iterator_insert_after(LinkedListIterator* iter, LinkedListNode* node)
{
    // IMPLEMENT THIS
    if(iter->curr == NULL)
    	return ERR_INSERT_AFTER_END;

    //iter->curr->next = node;
    node->next = iter->curr->next;
    iter->curr->next = node;

	return 0;
}

// Inserts node before the current node referenced by the iterator
// The iterator is valid after call and references the same object as before
void iterator_insert_before(LinkedListIterator* iter, LinkedListNode* node)
{
    // IMPLEMENT THIS
    *(iter->prev_next) = node;
    iter->prev_next = &(node->next);
    node->next = iter->curr;
}

//
// List functions
//

// Returns the maximum, minimum, and average price of the linked list
void max_min_avg_price(LinkedListNode** head, double* max, double* min, double* avg)
{
    // IMPLEMENT THIS
    LinkedListIterator iter;
    Object* obj;
    Object* init;
    double init_price;
    double obj_price;
    double price_sum = 0;
    int counter = 0;

    iterator_begin(&iter, head);
    
    init = iterator_get_object(&iter);
    init_price = object_price(init);

    *max = init_price;
    *min = init_price;

    while(iterator_at_end(&iter) == false)
	{
		obj = iterator_get_object(&iter);
		obj_price = object_price(obj);

		if(obj_price > *max)
			*max = obj_price;
		else if (obj_price < *min)
			*min = obj_price;
		
		price_sum += obj_price;
		counter++;
		iterator_next(&iter);
	}

	*avg = price_sum/counter;

} 

// Executes the func function for each node in the list
// The function takes in an input data and returns an output data, which is used as input to the next call to the function
// The initial input data is provided as a parameter to foreach, and foreach returns the final output data
// For example, if there are three nodes, foreach should behave like: return func(node3, func(node2, func(node1, data)))
Data foreach(LinkedListNode** head, foreach_fn func, Data data)
{
    // IMPLEMENT THIS
    // iterator at the head store the result in data d
    LinkedListIterator iter;
    Object* node;

    iterator_begin(&iter, head);

    while(iterator_at_end(&iter) == false)
	{
		node = iterator_get_object(&iter);
		data = func(node, data);
		iterator_next(&iter);
	}

    return data;
}

// Returns the length of the list
int length(LinkedListNode** head)
{
    // IMPLEMENT THIS
    LinkedListIterator iter;
    iterator_begin(&iter, head);
    int counter = 0;

    while(iterator_at_end(&iter) == false)
	{
		counter++;
		iterator_next(&iter);
	}

    return counter;
}

//
// Mergesort
//

// Assuming list1 and list2 are sorted lists, merge list2 into list1 while keeping it sorted
// The sort order is determined by the compare function
void merge(LinkedListNode** list1_head, LinkedListNode** list2_head, compare_fn compare)
{
    // IMPLEMENT THIS
    LinkedListIterator iter1;
    LinkedListIterator iter2;
    Object* obj1;
    Object* obj2;
    int result = 0;

    iterator_begin(&iter1, list1_head);
    iterator_begin(&iter2, list2_head);

    while(iterator_at_end(&iter2) != true && iterator_at_end(&iter1) != true)
	{
		obj1 = iterator_get_object(&iter1);
		obj2 = iterator_get_object(&iter2);
		result = compare(obj1,obj2);

		if(result <=  0)
		{
			//objects are equal
			//obj1 should be before obj2
			iterator_insert_after(&iter1, iter2.curr);
		}
		else
		{
			//obj1 should be after obj2
			iterator_insert_before(&iter1, iter2.curr);
		}

		iterator_remove(&iter2);
		iterator_next(&iter1);
	}
}

// Split the list head in half and place half in the split list
// For example, if head has 8 nodes, then split will move 4 of them to split_head
void split(LinkedListNode** head, LinkedListNode** split_head)
{

    // IMPLEMENT THIS
    LinkedListIterator iter;
    int len = 0;
    int counter = 0;
    int split = 0;

    iterator_begin(&iter, head);

    len = length(head);
    split = len/2;

    while(counter != split)
	{
		iterator_next(&iter);
		counter++;
	}

    *split_head = iter.curr;
    *iter.prev_next = NULL;

}

// Implement the mergesort algorithm to sort the list
// The sort order is determined by the compare function
void mergesort(LinkedListNode** head, compare_fn compare)
{
    // IMPLEMENT THIS
}
