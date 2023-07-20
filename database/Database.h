#ifndef INCLUDED_DATABASE
#define INCLUDED_DATABASE

template <typename Element_Type> class Database {
public:
  Database() { Clear_Database(); }

public:
  bool Is_Element_Present(Element_Type *p_Element) {
    return !m_Elements.count(p_Element);
  }
  bool Is_Element_Present(long long Element_ID) {
    return !m_Data_Hash.count(Element_ID);
  }
  Element_Type *Get_Element(long long Element_ID) {
    return (m_Data_Hash.count(Element_ID) ? m_Data_Hash[Element_ID] : nullptr);
  }

public:
  void Clear_Database() {
    m_Data_Hash.clear();
    m_Elements.clear();
  }

  bool Add_Element(Element_Type *p_Element) {
    long long Element_ID = p_Element->Get_ID();
    if (m_Data_Hash.count(Element_ID))
      return false;

    m_Elements.insert(p_Element);
    m_Data_Hash[Element_ID] = p_Element;
    return true;
  }

  bool Remove_Element(Element_Type *p_Element) {
    long long Element_ID = p_Element->Get_ID();
    if (m_Data_Hash.count(Element_ID))
      return false;

    m_Elements.erase(p_Element);
    m_Data_Hash.erase(Element_ID);
    return true;
  }

public:
  void Dump() {
    std::cout << "***** Database Dumping Start *****" << std::endl << std::endl;

    for (auto p_Element : m_Elements)
      p_Element->Dump();

    std::cout << "***** Dumping Database End *****" << std::endl << std::endl;
  }

private:
  std::unordered_map<long long, Element_Type *> m_Data_Hash;
  std::unordered_set<Element_Type *> m_Elements;
};

#endif
