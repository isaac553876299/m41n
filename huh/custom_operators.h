#ifndef CUSTOM_OPERATORS_H
#define CUSTOM_OPERATORS_H

// (c) 2008, John Crenshaw

// To create a new operator, you must:
// 1. Definition
// 2. Declare
// 3. DeclareOperatorLeftType // 3. DeclareOperatorLeftTypeRef
// 4. Implement functions (inline¿)
// 5. Bind
// But wonder, what good is syntactic sugar if it makes your app slow?


#define BinaryOperatorDefinition(opname, precedence) precedence CCustomOperatorHelper_##opname() precedence
#define UnaryPostOperatorDefinition(opname) * CCustomOperatorUnaryPostHelper_##opname()

class CCustomOperator_parambase
{
public:
    virtual ~CCustomOperator_parambase() {}
};

#define DeclareBinaryOperator(opname) \
template<class T_left> \
class CCustomOperatorHelper_##opname##_leftparam_T : public CCustomOperator_parambase \
{ public: \
   CCustomOperatorHelper_##opname##_leftparam_T(T_left l){m_l_buf = l;m_pl = &m_l_buf;} \
   T_left m_l_buf; /* buffer, since we have to copy the left side value */ \
   T_left* m_pl; /* pointer to the value on the left */ \
}; \
 \
template<class T_left> \
class CCustomOperatorHelper_##opname##_leftparamref_T : public CCustomOperator_parambase \
{ public: \
   CCustomOperatorHelper_##opname##_leftparamref_T(T_left& l){m_pl = &l;} \
   T_left* m_pl; /* pointer to the value on the left */ \
}; \
 \
class CCustomOperatorHelper_##opname \
{ public: \
   CCustomOperatorHelper_##opname(){m_pLeft = 0/*null*/;} \
   ~CCustomOperatorHelper_##opname(){delete m_pLeft;} \
   CCustomOperator_parambase* m_pLeft; \
}; \

#define DeclareUnaryPostOperator(opname) \
class CCustomOperatorUnaryPostHelper_##opname \
{ public: \
   virtual ~CCustomOperatorUnaryPostHelper_##opname(){} \
}; \

#define DeclareOperatorLeftType(opname, precedence, lefttype) \
inline CCustomOperatorHelper_##opname##_leftparam_T<lefttype>& operator precedence (lefttype l, CCustomOperatorHelper_##opname& r) \
{ return *(CCustomOperatorHelper_##opname##_leftparam_T<lefttype>*)(r.m_pLeft = new CCustomOperatorHelper_##opname##_leftparam_T<lefttype>(l)); } \

#define DeclareOperatorLeftTypeRef(opname, precedence, lefttype) \
inline CCustomOperatorHelper_##opname##_leftparamref_T<lefttype>& operator precedence (lefttype& l, CCustomOperatorHelper_##opname& r) \
{ return *(CCustomOperatorHelper_##opname##_leftparamref_T<lefttype>*)(r.m_pLeft = new CCustomOperatorHelper_##opname##_leftparamref_T<lefttype>(l)); } \

#define BindBinaryOperator(rettype, opname, precedence, lefttype, righttype) \
inline rettype operator precedence (CCustomOperatorHelper_##opname##_leftparam_T<lefttype>& l, righttype r) \
{ return opname(*l.m_pl, r); } \
inline rettype operator precedence (CCustomOperatorHelper_##opname##_leftparamref_T<lefttype>& l, righttype r) \
{ return opname(*l.m_pl, r); } \

#define BindUnaryPostOperator(rettype, opname, lefttype) \
inline lefttype operator * (lefttype l, CCustomOperatorUnaryPostHelper_##opname& r) \
{ return opname(l); } \

// end

// sample operator
#define swap BinaryOperatorDefinition(_op_swap,>>=)
DeclareBinaryOperator(_op_swap);
DeclareOperatorLeftType(_op_swap, >>=, int);
void _op_swap(int a, int b)
{
    int& c = a;
    a = b;
    b = c;
}
BindBinaryOperator(void, _op_swap, >>=, int, int);


#endif
