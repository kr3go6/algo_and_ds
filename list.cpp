#include <initializer_list>

namespace nonstd
{
    template <typename ValType>
    class SingleLinkedList
    {
        struct ListNode
        {
            ValType val;
            ListNode *next;
            ListNode(ValType v = 0, ListNode *n = nullptr) : val(v), next(n) {}
        };

        ListNode *start_p;
        unsigned sz;

    public:
        using value_type = ValType;

        SingleLinkedList() : start_p(nullptr), sz(0) {}
        SingleLinkedList(const SingleLinkedList<ValType>& orig);
        SingleLinkedList(const std::initializer_list<ValType> init_list);
        ~SingleLinkedList();

        ListNode* begin(void) { return start_p; }
        // ListNode<ValType>
        // insert()
    };

    template <typename ValType>
    SingleLinkedList<ValType>::SingleLinkedList(
            const SingleLinkedList<ValType>& orig) : start_p(nullptr), sz(0)
    {
        if (orig.start_p) {
            ValType *tmp_orig_p = orig.start_p, *tmp_p = start_p = new ListNode;
            
            while (tmp_orig_p) {
                tmp_p->val = tmp_orig_p->val;

                if (tmp_orig_p = tmp_orig_p->next) {
                    tmp_p = tmp_p->next = new ListNode;
                }
            }
        }
    }

    template <typename ValType>
    SingleLinkedList<ValType>::SingleLinkedList(
            const std::initializer_list<ValType> init_list)
    {
        if (init_list.size()) {
            ListNode *cur = new ListNode;
            start_p = cur;

            for (const ValType *it = init_list.begin(); it != init_list.end(); 
                    cur = cur->next = new ListNode) {
                cur->val = *it;
                it++;
            }
        }
    }

    template <typename ValType>
    SingleLinkedList<ValType>::~SingleLinkedList()
    {
        ListNode *nxt = start_p->next;

        while (start_p) {
            delete start_p;
            start_p = nxt;
            if (nxt) nxt = nxt->next;
        }
    }
}

#include <iostream>

int main(void)
{
    nonstd::SingleLinkedList<double> l1 = {1, 2, 3};
    // std::cout << l1.begin()->next->val;
}