#ifndef _QX_BICYCLES_BICYCLE_H_
#define _QX_BICYCLES_BICYCLE_H_

class QX_BICYCLES_DLL_EXPORT bicycle
{
public:
// -- properties
   long           m_id;
   QString        m_brand;
   QString        m_model;

// -- contructor, virtual destructor
   bicycle() : m_id(0) { ; }
   virtual ~bicycle() { ; }
};

QX_REGISTER_HPP_QX_BICYCLES(bicycle, qx::trait::no_base_class_defined, 0)

typedef std::shared_ptr<bicycle> bicycle_ptr;
typedef std::vector<bicycle_ptr> list_bicycle;

#endif // _QX_BICYCLES_BICYCLE_H_
