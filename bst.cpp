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
        friend BSTNode<VT>* INS(BSTNode<VT>*&, const VT&, std::size_t&, std::size_t) noexcept;

        using value_type = ValueType;
        constexpr value_type value() const noexcept { return val; }
        constexpr BSTNode* get_left() const noexcept { return left; }
        constexpr BSTNode* get_right() const noexcept { return right; }
    private:
        ValueType val;
        BSTNode *left;
        BSTNode *right;

        BSTNode(const BSTNode& orig) noexcept;
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
    INS(BSTNode<ValueType>*& p_head, const ValueType& v, std::size_t& h, std::size_t tmp_h) noexcept
    {
        if (!p_head) {
            p_head = new BSTNode<ValueType>(v);
            if (tmp_h > h) h = tmp_h;
            return p_head;
        }

        if (v < p_head->val) {
            return INS(p_head->left, v, h, tmp_h + 1);
        } else {
            return INS(p_head->right, v, h, tmp_h + 1);
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


    template <typename ValueType>
    class BinarySearchTree
    {
        BSTNode<ValueType> *start_p;
        std::size_t sz;
        std::size_t hght;
        BinarySearchTree<ValueType>& operator=(const BinarySearchTree<ValueType>& rhs) noexcept;
        BinarySearchTree(const BinarySearchTree<ValueType>& orig) noexcept;

    public:
        using value_type = ValueType;
        using reference = value_type&;
        using const_reference = const reference;

        BinarySearchTree() noexcept : start_p(nullptr), sz(0), hght(0) {}
        BinarySearchTree(const std::initializer_list<value_type>) noexcept;
        ~BinarySearchTree() noexcept;

        BSTNode<value_type>*
        insert(value_type v) noexcept { ++sz; return INS(start_p, v, hght, 1); }

        BSTNode<value_type>* top() { return start_p; }
        constexpr std::size_t size() noexcept { return sz; }
        constexpr std::size_t height() noexcept { return hght; }
        void clear() noexcept { this->~BinarySearchTree(); }
    };

    template <typename ValueType>
    BinarySearchTree<ValueType>::~BinarySearchTree() noexcept
    {
        DEL(start_p);
        sz = hght = 0;
    }

    template <typename ValueType>
    BinarySearchTree<ValueType>::BinarySearchTree(
            const std::initializer_list<value_type> init_list) noexcept : start_p(nullptr), sz(0), hght(0) 
    {
        for (auto v : init_list) {
            this->insert(v);
        }
    }
}