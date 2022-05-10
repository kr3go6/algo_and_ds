#include <algorithm>
#include <cstddef>

namespace nonstd
{
    template <typename ValueType> struct BSTNode;
    template <typename ValueType> class BinarySearchTree;

    template <typename ValueType>
    struct BSTNode
    {
        friend class BinarySearchTree<ValueType>;
        
        template <typename VT>
        friend void DEL(BSTNode<VT>*&) noexcept;

        template <typename VT>
        friend BSTNode<VT>* INS(BSTNode<VT>*&, const VT&) noexcept;

        template <typename VT>
        friend int RMV(BSTNode<VT>*&, const VT&) noexcept;

        template <typename VT>
        friend BSTNode<VT>*& minimum(BSTNode<VT>*& p_head) noexcept { 
                if (!p_head->left) return p_head; return minimum(p_head->left); }

        template <typename VT, typename FT>
        friend void INORD(BSTNode<VT>*&, FT);

        template <typename VT, typename FT>
        friend void PREORD(BSTNode<VT>*&, FT);

        using value_type = ValueType;
        constexpr value_type value() const noexcept { return val; }
        constexpr BSTNode* get_left() const noexcept { return left; }
        constexpr BSTNode* get_right() const noexcept { return right; }
    private:
        ValueType val;
        BSTNode *left;
        BSTNode *right;

        BSTNode(const BSTNode& orig) noexcept : val(orig.val), left(orig.left), right(orig.right) {}
        BSTNode(ValueType v) noexcept : val(v), left(nullptr), right(nullptr) {}
        BSTNode(ValueType v, BSTNode *lft, BSTNode *rght) noexcept : 
                val(v), left(lft), right(rght) {}
        BSTNode(BSTNode&& rhs) noexcept : left(nullptr), right(nullptr)
        {
            std::swap(val, rhs.val); 
            std::swap(left, rhs.left);
            std::swap(right, rhs.right);
        }
    };

    template <typename ValueType>
    void 
    DEL(BSTNode<ValueType>*& p_head) noexcept 
    { 
        if (p_head) {
            DEL(p_head->left);
            DEL(p_head->right);
            delete p_head;
            p_head = nullptr;
        }
    }

    template <typename ValueType>
    BSTNode<ValueType>* 
    INS(BSTNode<ValueType>*& p_head, const ValueType& v) noexcept
    {
        if (!p_head) {
            p_head = new BSTNode<ValueType>(v);
            return p_head;
        }

        if (v < p_head->val) {
            return INS(p_head->left, v);
        } else {
            return INS(p_head->right, v);
        }
    }

    template <typename ValueType>
    BSTNode<ValueType>* 
    FIND(BSTNode<ValueType>* p_head, const ValueType& v) noexcept
    {
        if (!p_head) return nullptr;

        if (v < p_head->val) {
            return FIND(p_head->left, v);
        } else {
            return FIND(p_head->right, v);
        }
    }

    // returns -1 if element was not found; else 0
    template <typename ValueType>
    int
    RMV(BSTNode<ValueType>*& p_head, const ValueType& v) noexcept
    {
        if (!p_head) {
            return -1;
        }

        if (v < p_head->val) {
            return RMV(p_head->left, v);
        } else if (v > p_head->val) {
            return RMV(p_head->right, v);
        }

        if (!p_head->left && !p_head->right) {
            DEL(p_head);
            return 0;
        }

        if (p_head->left && !p_head->right) {
            BSTNode<ValueType> *tmp = p_head;
            p_head = p_head->left;
            delete tmp;
            return 0;
        }

        if (!p_head->left) {
            BSTNode<ValueType> *tmp = p_head;
            p_head = p_head->right;
            delete tmp;
            return 0;
        }

        if (!p_head->right->left) {
            BSTNode<ValueType> *tmp = p_head;
            p_head->right->left = p_head->left;
            p_head = p_head->right;
            delete tmp;
            return 0;
        }

        BSTNode<ValueType> *new_head = minimum(p_head->right);
        p_head->val = new_head->val;
        return RMV(minimum(p_head->right), new_head->val);
    }

    template <typename ValueType, typename Function>
    void
    INORD(BSTNode<ValueType>*& p_head, Function f)
    {
        if (!p_head) return;

        INORD(p_head->left, f);
        f(p_head->val);
        INORD(p_head->right, f);
    }

    template <typename ValueType, typename Function>
    void
    PREORD(BSTNode<ValueType>*& p_head, Function f)
    {
        if (!p_head) return;

        f(p_head->val);
        PREORD(p_head->left, f);
        PREORD(p_head->right, f);
    }


    template <typename ValueType>
    class BinarySearchTree
    {
        BSTNode<ValueType> *start_p;
        std::size_t sz;
        BinarySearchTree<ValueType>& operator=(const BinarySearchTree<ValueType>& rhs) noexcept;
        BinarySearchTree(const BinarySearchTree<ValueType>& orig) noexcept;

    public:
        using value_type = ValueType;
        using reference = value_type&;
        using const_reference = const reference;

        template <typename VT, typename FT>
        friend void inorder(BinarySearchTree<VT>&, FT);

        template <typename VT, typename FT>
        friend void preorder(BinarySearchTree<VT>&, FT);

        BinarySearchTree() noexcept : start_p(nullptr), sz(0) {}
        BinarySearchTree(const std::initializer_list<value_type>) noexcept;
        ~BinarySearchTree() noexcept;

        BSTNode<value_type>*
        insert(value_type v) noexcept { ++sz; return INS(start_p, v); }

        int
        remove(value_type v) noexcept { int res = RMV(start_p, v); sz -= res + 1; return res; }

        BSTNode<value_type>* top() { return start_p; }
        constexpr std::size_t size() noexcept { return sz; }
        void clear() noexcept { this->~BinarySearchTree(); }
    };

    template <typename ValueType>
    BinarySearchTree<ValueType>::~BinarySearchTree() noexcept
    {
        DEL(start_p);
        sz = 0;
    }

    template <typename ValueType>
    BinarySearchTree<ValueType>::BinarySearchTree(
            const std::initializer_list<value_type> init_list) noexcept : start_p(nullptr), sz(0)
    {
        for (auto v : init_list) {
            this->insert(v);
        }
    }

    template <typename ValueType, typename Function>
    void
    inorder(BinarySearchTree<ValueType>& bst, Function f)
    {
        INORD(bst.start_p, f);
    }

    template <typename ValueType, typename Function>
    void
    preorder(BinarySearchTree<ValueType>& bst, Function f)
    {
        PREORD(bst.start_p, f);
    }
}