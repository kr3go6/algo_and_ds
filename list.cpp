#include <algorithm>
#include <cstddef>
#include <iostream>


namespace nonstd
{
    template <typename ValueType> struct ListNode;
    template <typename ValueType> class SingleLinkedList;

    template <typename ValueType>
    struct ListNode
    {
        friend class SingleLinkedList<ValueType>;
    private:
        ValueType val;
        ListNode *next_;

        ListNode(const ListNode& orig) noexcept;
        ListNode(ValueType v = 0, ListNode *n = nullptr) noexcept : val(v), next_(n) {}
        ListNode(ListNode&& rhs) noexcept : val(0), next_(nullptr)
        { 
            std::swap(val, rhs.val); std::swap(next_, rhs.next_); 
        }
    public:
        using value_type = ValueType;
        constexpr ValueType value() const noexcept { return val; }
        constexpr ListNode* next() const noexcept { return next_; }
    };


    template <typename ValueType>
    class SingleLinkedList
    {
        ListNode<ValueType> *start_p;
        std::size_t sz;

    public:
        using value_type = ValueType;
        using reference = value_type&;
        using const_reference = const value_type&;

        SingleLinkedList() noexcept : start_p(nullptr), sz(0) {}
        SingleLinkedList(const SingleLinkedList<value_type>& orig) noexcept;
        SingleLinkedList(const std::initializer_list<value_type> init_list) noexcept;
        ~SingleLinkedList() noexcept;

        SingleLinkedList<value_type>& operator=(const SingleLinkedList<value_type>& rhs) noexcept;

        ListNode<value_type>* begin() { return start_p; }
        constexpr value_type size() const noexcept { return sz; }
        constexpr bool empty() const noexcept { return !sz; }
        void clear() noexcept { this->~SingleLinkedList(); }
    };

    template <typename ValueType>
    SingleLinkedList<ValueType>&
    SingleLinkedList<ValueType>::operator=(const SingleLinkedList<value_type>& orig) noexcept
    {
        start_p = nullptr;
        sz = 0;

        if (orig.start_p) {
            ListNode<value_type> *tmp_orig_p = orig.start_p, *tmp_p = start_p = new ListNode<value_type>;
            
            while (tmp_orig_p && ++sz) {
                tmp_p->val = tmp_orig_p->val;

                if ((tmp_orig_p = tmp_orig_p->next_)) {
                    tmp_p = tmp_p->next_ = new ListNode<value_type>;
                }
            }
        }

        return *this;
    }

    template <typename ValueType>
    SingleLinkedList<ValueType>::SingleLinkedList(
            const SingleLinkedList<value_type>& orig) noexcept : start_p(nullptr), sz(0)
    {
        *this = orig;
    }

    template <typename ValueType>
    SingleLinkedList<ValueType>::SingleLinkedList(
            const std::initializer_list<value_type> init_list) noexcept
    {
        sz = init_list.size();

        if (sz) {
            ListNode<value_type> *cur = new ListNode<value_type>;
            start_p = cur;

            for (const value_type *it = init_list.begin(); it != init_list.end();) {
                cur->val = *it;
                it++;

                if (it != init_list.end()) {
                    cur = cur->next_ = new ListNode<value_type>;
                }
            }
        }
    }

    template <typename ValueType>
    SingleLinkedList<ValueType>::~SingleLinkedList() noexcept
    {
        sz = 0;

        if (start_p) {
            ListNode<value_type> *nxt = start_p->next_;

            while (start_p) {
                delete start_p;
                start_p = nxt;
                if (nxt) nxt = nxt->next_;
            }

        }
    }

    void printList(SingleLinkedList<double>& l1) {
        ListNode<double> *tmp = l1.begin();

        while (tmp) {
            std::cout << tmp << " " << tmp->value() << " " << tmp->next() << std::endl;
            tmp = tmp->next();
        }
    }
}


int main(void)
{
    // using namespace nonstd;

    nonstd::SingleLinkedList<int> my_list;
    my_list = {1, 2, 3, 4, 5};
    my_list.clear();
    std::cout << my_list.empty() << std::endl;

    return 0;
}