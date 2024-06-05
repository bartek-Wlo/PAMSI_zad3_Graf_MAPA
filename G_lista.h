/*
https://eduarmandov.files.wordpress.com/2017/05/c_c-data-structures-and-algorithms-in-c.pdf#page=145
*/

#ifndef G_LISTA_H
#define G_LISTA_H

#include <stdbool.h> //bool
#include <iostream>
using namespace std;
template <typename E> class lista; // Wstępna deklaracja szablonu klasy lista

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/ Klasa  node \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E>
class node { /* pojedynczy elemnet listy */
private:
  E elem;
  node<E> *next; /* Wskaźnik typu klasy wagon na następny element listy */
  friend class lista<E>;
};

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/ Klasa lista \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E>
class lista {

public:
  lista() : head(nullptr),node_number(0) {}
  ~lista() { while (!empty()) removeFront(); }
  bool empty() const;              // czy lista jest pusta?
  const E &front() const;          // zwróć pierwszy element.
  void addFront(const E &dane);    // Dodaj dane na początek listy.
  void removeFront();              // Usuń pierwszy element listy.
  void removeInside(node<E> *ptr); // Usuń element na liście. ZŁOŻONOŚC O(n)

  int get_nodeNumber() const {return node_number;}
  const E& get_elem(const node<E>* ptr) const;
  node<E>* get_head() const {return head;}
  node<E>* get_next(node<E>* ptr) const;

private:
  node<E> *head;
  unsigned int node_number;
};

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/  Definicje funkcji  \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/

template <typename E>
bool lista<E>::empty() const {
  return head == nullptr; 
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E>
const E& lista<E>::front() const {
  if (empty()) {
    throw std::out_of_range
      ("Returning empty list.\n           G_lista.h -> front()");
  }
  return head->elem;
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E> 
void lista<E>::addFront(const E& dane) { // add to front of list
  node<E>* v = new node<E>;
  v->elem = dane;
  v->next = head;
  head = v;
  ++node_number;
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E>
void lista<E>::removeFront() {
  if (empty()) {
    throw std::out_of_range
      ("Removing empty list.\n           G_lista.h -> removeFront()");
  }
  node<E>* old = head;
  head = old->next;
  delete old;
  --node_number;
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E> /* Złożoność O(n) */
void lista<E>::removeInside(node<E> *ptr_to_elem_being_removed) {
  node<E> *looking /* for ptr->next == ptr_to_elem_being_removed */ = head;
  if (empty()) {
    throw std::out_of_range
      ("Removing in empty list.\n           G_lista.h -> removeInside()");
  }
  if (head == ptr_to_elem_being_removed) {
    removeFront();
    return;
  }
  while (looking->next != ptr_to_elem_being_removed) {
    looking = looking->next;
    if (looking->next == nullptr) throw std::invalid_argument 
      ("ptr_to_elem_being_removed is not on this list\n           G_lista.h -> removeInside(node<E>* ptr_to_elem_being_removed)");
  }
  looking->next = ptr_to_elem_being_removed->next;
  delete ptr_to_elem_being_removed;
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E>
const E& lista<E>::get_elem(const node<E>* ptr) const {
  if (empty()) {
    throw std::out_of_range
      ("Returning in empty list.\n           G_lista.h -> get_elem(node<E>*)");
  }
  if (ptr == nullptr) {
    throw std::invalid_argument 
      ("node<E>* ptr == nullptr\n           G_lista.h -> get_elem(node<E>*)");
  }
  return ptr->elem;
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
template <typename E>
node<E>* lista<E>::get_next(node<E>* ptr) const {
  if (empty()) {
    throw std::out_of_range
      ("Moving in empty list.\n           G_lista.h -> get_next(node<E>*)");
  }
  if (ptr == nullptr) {
    throw std::invalid_argument
      ("node<E>* ptr == nullptr\n           G_lista.h -> get_next(node<E>*)");
  }
  return ptr->next;
};

#endif