# pragma once

#include <vector>
#include <algorithm>
#include <cstddef>


namespace taskman {


template <typename PayloadType, typename ComparatorType>
class Heap
{
  public:
    Heap()
    {}
  
    Heap(std::vector<PayloadType> const & initialElements)
      : m_backend(initialElements)
    {
      std::make_heap(m_backend.begin(), m_backend.end(), m_comparator);
    }

    PayloadType & top()
    {
      return m_backend.front();
    }

    PayloadType const & top() const
    {
      return m_backend.front();
    }

    void push(PayloadType const & payload)
    {
      m_backend.push_back(payload);
      std::push_heap(m_backend.begin(), m_backend.end(), m_comparator);
    }
    
    void pop()
    {
      std::pop_heap(m_backend.begin(), m_backend.end(), m_comparator);
      m_backend.pop_back();
    }

    std::vector<PayloadType> const & getAllElements() const
    {
      return m_backend;
    }
    
    std::size_t size() const
    {
      return m_backend.size();
    }

    void clear() {
      m_backend.clear();
    }

  protected:
    std::vector<PayloadType> m_backend;
    ComparatorType           m_comparator;
};


} // namespace taskman
