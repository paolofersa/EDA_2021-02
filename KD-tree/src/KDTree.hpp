// Copyright

#ifndef SRC_KDTREE_HPP_
#define SRC_KDTREE_HPP_

#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>
#include <queue>
#include "Point.hpp"
using namespace std;

template <size_t N, typename ElemType>
struct KDNode
{
    Point<N> point;
    int depth;
    ElemType value;
    KDNode<N, ElemType>* children[2];
    KDNode(Point<N> _point, ElemType _value, int _depth) {
        point = _point;
        depth = _depth;
        value = _value;
        children[0] = children[1] = 0;
    }
};

template <size_t N, typename ElemType>  //dimensiones y tipo de dato
class KDTree {
 public:
  typedef pair<Point<N>, ElemType> value_type;

  KDTree();

  ~KDTree();

  KDTree(const KDTree &rhs);    //constructor copia
  KDTree &operator=(const KDTree &rhs);

  size_t dimension() const;     //retornar cantidad de dimensiones N

  size_t size() const;          //cantidad de elementos
  bool empty() const;           //vacío

  bool contains(const Point<N> &pt) const;  //si contine un punto

  void insert(const Point<N> &pt, const ElemType &value);   //insertar un punto (recibe punto y valor del elemento)

  ElemType &operator[](const Point<N> &pt); //retorna valor del punto

  ElemType &at(const Point<N> &pt);     //también retorna valor del punto
  const ElemType &at(const Point<N> &pt) const;     //lo mismo que el anterior pero para KDtree constante

  ElemType knn_value(const Point<N> &key, size_t k) const;  //knn mas cercano, retorna el valor mas comun de los k mas cercanos

  vector<ElemType> knn_query(const Point<N> &key, size_t k) const;      //devuelve vector con los vecinos mas cercanos

 private:
  size_t dimension_;        //dimension
  size_t size_;             //cantidad de elementos
  KDNode<N, ElemType>* root;
};

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    dimension_ = N;
    size_ = 0;
    root = nullptr;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
  // TODO(me): Fill this in.
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree &rhs) {
    dimension_ = rhs.dimension_;
    size_ = rhs.size_;
    root = nullptr;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType> &KDTree<N, ElemType>::operator=(const KDTree &rhs) {
    dimension_ = rhs.dimension_;
    size_ = rhs.size_;
    root = rhs.root;
    return *this;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return dimension_;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return size_;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    if (size_)
        return false;
    return true;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N> &pt) const {
    KDNode<N, ElemType>*const* p;
    int _depth = 0;
    for (p = &root;
        *p && (*p)->point != pt;
        p = &((*p)->children[pt[_depth % N] > (*p)->point[_depth % N]]), _depth++);
    return *p != 0;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N> &pt, const ElemType &value) {
    KDNode<N, ElemType>** p = nullptr;
    int _depth = 0;
    for (p = &root;
        *p && (*p)->point != pt;
        p = &((*p)->children[pt[_depth % N] > (*p)->point[_depth % N]]), _depth++);
    if (*p == 0) {
        *p = new KDNode<N, ElemType>(pt,value, _depth);
        size_++;
    }
    else
        (*p)->value = value;
}

template <size_t N, typename ElemType>
ElemType &KDTree<N, ElemType>::operator[](const Point<N> &pt) {
    KDNode<N, ElemType>** p = nullptr;
    int _depth = 0, value = 0;
    for (p = &root;
        *p && (*p)->point != pt;
        p = &((*p)->children[pt[_depth % N] > (*p)->point[_depth % N]]), _depth++);
    if (*p == 0) {
        *p = new KDNode<N, ElemType>(pt, value, _depth);
        size_++;
    }
    return (*p)->value;
}

template <size_t N, typename ElemType>
ElemType &KDTree<N, ElemType>::at(const Point<N> &pt) {
    KDNode<N, ElemType>*const* p;
    int _depth = 0;
    for (p = &root;
        *p && (*p)->point != pt;
        p = &((*p)->children[pt[_depth % N] > (*p)->point[_depth % N]]), _depth++);
    if (*p != 0)
        return (*p)->value;
}

template <size_t N, typename ElemType>
const ElemType &KDTree<N, ElemType>::at(const Point<N> &pt) const {
    KDNode<N, ElemType>* const* p;
    int _depth = 0;
    for (p = &root;
        *p && (*p)->point != pt;
        p = &((*p)->children[pt[_depth % N] > (*p)->point[_depth % N]]), _depth++);
    if (*p != 0)
        return (*p)->value;
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::knn_value(const Point<N> &key, size_t k) const {
  // TODO(me): Fill this in.
  ElemType new_element;
  return new_element;
}

template <size_t N, typename ElemType>
vector<ElemType> KDTree<N, ElemType>::knn_query(const Point<N> &key, size_t k) const {
  // TODO(me): Fill this in.
  vector<ElemType> values;
  return values;
}

// TODO(me): finish the implementation of the rest of the KDTree class

#endif  // SRC_KDTREE_HPP_
