#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
1. [] () . ->
2. (typecast) &
*/
//#define offsetof(TYPE, MEMBER) ((unsigned int)&((TYPE*)0)->(MEMBER))
#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#if 0
/*
if no "_ptr;" at the end of container_of(ptr, type, member), 
the function is ok but there is warning when compile: 
variable '_ptr' set but not used [-Wunused-but-set-variable]
*/
#define container_of(ptr, type, member) \
({\
	type *_ptr;					\
	_ptr = (type*)((int)(ptr) - (int)(&((type*)NULL)->member));\
	_ptr;\
})
#else
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
		(type *)( (char *)__mptr - offsetof(type,member) );})
#endif		

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

struct list_head {
	struct list_head *next, *prev;
};

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->prev = list;
	list->next = list;
}

//#define LIST_HEAD_INIT(name) {.prev = &name, .next = &name,}
#define LIST_HEAD_INIT(name) {&name,&name}
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

/*
the following method forgets LIST_HEAD_INIT
#define LIST_HEAD(name) struct list_head name = {&name, &name}
*/
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}
/*
1. when list_add one 'new' entry to an empty list, prev == next,
2. these 4 assignments sequence doesnot matter;
3. new's fields are not necessary to be initialized. What its initial value is doesnot matter.
*/
static inline void __list_add(struct list_head *new, 
				struct list_head *prev,
				struct list_head *next)
{
	new->next = next;
	new->prev = prev;
	next->prev = new;
	prev->next = new;
}
/*
'new' will be the next element after 'head', list_add_xx seems to be marriage. Whether new is 
initialized(virgin) doesnot matter:)
*/
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}
//'new' is the at the prev position of head,"new" 's next is 'head'
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}
/*
1. "prev==next" when there is only one element remaining in list
2. these 2 assignments sequence doesnot matter
3. the deleted element's fields are not changed
*/
static inline void __list_del(struct list_head *prev, struct list_head *next)
{ 
	prev->next = next;
	next->prev = prev;
}
/*
after deletion, "entry"'s prev/next is not NULL! In other words, list_del seems to be divorce, the deleted 
"entry" is not an virgin:)
***PAY ATTENTION:
the real kernel's list_del will poinson the entry's prev&next
*/
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}
/*
list_replace is just a re-marriage, old becomes a non-virgin:its old .prev/.next is not changed at all,
whether new is virgin(what its .prev/.next is doesnot matter) doesnot matter!
1. old->prev==old->next is possible when there is only one element remaining in link-list! see 2 for detail.
2. 
 a)the link-list old belongs to cannot be empty! if new!=old and old is in an empty link-list, list_replace 
   with the following sequence:
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;   
   will generate an self-linked "new" and the following:
   old->prev==new, old->next==old, old's self link-list is destroyed!
 b)in normal case, old belongs to one non-empty list(old<=>next), new doesnot belong to old's list. With the the above a's sequence, it will generate new<=>next, old->prev/next is not changed at all.
 
 So old->prev==old->next is possible when there is only one element remaining in link-list!
 
 the 4 assignment sequence doesnot matter since old->next/prev will not be changed after list_replace. In 
 other words, it can be:
	old->next->prev = new;
	old->prev->next = new;
	new->prev = old->prev;
	new->next = old->next;
3. after list_replace, the old's .prev/.next is not changed at all. Besides, new's .prev/.next initial value 
   doesnot matter, which will be overwritten by new value.
*/

static inline void list_replace(struct list_head *old, struct list_head *new)
{
#if 0
	struct list_head *prev = old->prev, *next = old->next;
	__list_add(new, prev, next);
#else
	//four assignments sequence doesnot matter!
	new->prev = old->prev;
	new->next = old->next;
	old->next->prev = new;
	old->prev->next = new;
#endif	
}
//old's link-list cannot be empty while new can be an self-linked empty list, see comments of list_replace
static inline void list_replace_init(struct list_head *old, struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}

static inline void list_del_init(struct list_head *entry)
{
	list_del(entry);
#if 0	
	entry->next = entry;
	entry->prev = entry;
#else
	INIT_LIST_HEAD(entry);
#endif
}
/*
"list" is removed from its list A and added in "head"'s list B(In B, "list" will be "head"'s next)
A and B can be the same, or not.
if both list/head is empty, list_move will generate head<=>list
*/
static inline void list_move(struct list_head *list, struct list_head *head)
{
	/*
	no need to use list_del_init since list_add will not use "list"'s original .prev and .next;
	It doesnot matter what's the initial .prev/.next value of "list" is. So the time of initializing
	"list" can be saved.
	*/
	list_del(list);//list_del will not change .prev/.next of "list"
	list_add(list, head);
}

static inline void list_move_tail(struct list_head *list, struct list_head *head)
{
	list_del(list);
	list_add_tail(list, head);
}
/*
'first/last' 's link-list shoud not be the same one of 'prev/next'. But the following can be true:
1. prev==next or first==last or (prev==next && first==last)
2. the 4 assignments sequence doesnot matter
3. list->prev/next is not changed at all after __list_splice
*/
static inline void __list_splice(const struct list_head *list,
				struct list_head *prev,
				struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	prev->next = first;
	first->prev = prev;
	next->prev = last;
	last->next = next;
}
//till finish, "list"'s fields .prev/.next are the same as before, not changed at all
static inline void list_splice(const struct list_head *list, 
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}
/*
when "list"'s list contains only one element and head is an empty link-list, its __list_splice will be:
prev==next since head belongs to an empty link-list;
first==last since list->next==list->prev
*/
static inline void list_splice_init(struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}
/*
list can not be NULL while head can be NULL
if "list" belongs to A while "head" belongs to B, A and B should not the same link-list
*/
static inline void list_splice_tail(const struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next;\
		pos != (head);\
		pos = n, n = n->next)//or n = pos->next

#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev;\
		pos != (head);\
		pos = n, n = pos->prev)
		
#define list_for_each_entry(pos, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos) ,member);\
		 &pos->member != (head);\
		 pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_reverse(pos, head, member) \
	for (pos = list_entry((head)->prev, typeof(*pos), member);\
		&pos->member != (head);\
		pos = list_entry(pos->member.prev, typeof(*pos), member))
		 
/*
since list_del(pos) is OK! pos->next & pos->prev will be invalid after
deletion. So preserve the next pointer via n before "loop operations";
*/
#define list_for_each_entry_safe(pos, n, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos), member), \
		n = list_entry(pos->member.next, typeof(*pos), member); \
		&pos->member != head;\
		pos = n, n = list_entry(n->member.next, typeof(*n), member))

#define list_for_each_entry_safe_reverse(pos, n, head, member) \
	for (pos = list_entry((head)->prev, typeof(*pos), member),\
		n = list_entry(pos->member.prev, typeof(*n), member);\
		&pos->member != (head);\
		pos = n, n = list_entry(n->member.prev,typeof(*n),member))

/////////////
struct string {
	char *name;
	struct list_head list;
	void *data;
};
static LIST_HEAD(arg_list);
struct string g_str = {
	.name = "red",
	.list = LIST_HEAD_INIT(g_str.list),
	.data = NULL,
};
////////////
static LIST_HEAD(unwind_tables);

void list_build(int argc, char **argv)
{
	int i;
	struct string *pos, *next;
	//MP8 is looking for rednoah linux
	for (i = 1; i < argc; i++) {
		char *src = argv[i];
		struct string * str = malloc(sizeof(struct string));
		str->name = strdup(src);
		list_add_tail(&str->list, &unwind_tables);
		printf("%s\t%p\n", str->name, str);
	}
	list_for_each_entry_safe(pos, next, &unwind_tables, list) {
		printf("%s ", pos->name);
		if (!strcmp(pos->name, "MP8")) {
			list_del(&pos->list);
			free(pos->name);
			free(pos);
		}else if (!strcmp(pos->name, "rednoah")) {
			list_move(&pos->list, &unwind_tables);
		}
	}

	printf("\nuse next:");//rednoah is looking for linux
	list_for_each_entry(pos, &unwind_tables,list) {
		printf("%s ", pos->name);
	}
	printf("\nreverse:");//the reverse of the above
	list_for_each_entry_reverse(pos, &unwind_tables, list) {
		printf("%s ", pos->name);
	}

	printf("\nbefore :list empty %d\n", list_empty(&unwind_tables));
	list_for_each_entry_safe(pos, next, &unwind_tables, list) {
		list_del(&pos->list);
		free(pos->name);
		free(pos);
	}
	
	printf("after :list empty %d\n", list_empty(&unwind_tables));
}
int main(int argc, char **argv)
{
	struct string str, *p = &str;
	p->name = "123";
	INIT_LIST_HEAD(&p->list);
	p->data = NULL;
	printf("list_empty %d\n", list_empty(&p->list));
	printf("list_empty %d\n", list_empty(&g_str.list));
	printf("%p\n", p);
	//printf("%p\n", p->(name));//expected identifier before '(' token
	printf("%p\n", container_of(&str.name, struct string, name));
	container_of(&str.name, struct string, name)->name = "abc";
	printf("%p\n", container_of(&str.list, struct string, list));
	printf("%p\n", container_of(&str.data, struct string, data));
	printf("%p ", container_of(&str.list.prev, struct string, list.prev));
	printf("%p\n", list_entry(str.list.prev, struct string, list));
	printf("%d %d\n", offsetof(struct string, list.prev), offsetof(struct string, list.next));
	list_build(argc, argv);
	return 0;
}
