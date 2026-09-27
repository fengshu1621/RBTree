#include<iostream>
using namespace std;
template<typename T>
class RBTree{
public:
RBTree():root_(nullptr){}
//插入操作
void insert(const T &val)
{
    if(root_==nullptr)
    {
        root_=new Node(val);
        return;
    }
    Node*parent=nullptr;
    Node*cur=root_;
    while(cur!=nullptr)
    {
        if(cur->data_>val)
        {
            parent=cur;
            cur=cur->left_;
        }
        else if (cur->data_<val)
        {
            parent=cur;
            cur=cur->right_;
        }
        else
        {
            return;
        }
    }
    //设置当前节点的parent和颜色
    Node*node=new Node(val,parent,nullptr,nullptr,RED);
    if(parent->data_>val)
    {
        parent->left_=node;
    }
    else
    {
        parent->right_=node;
    }
    //如果新插入的红色节点，父节点也是红色节点，则调整
    if(RED==color(parent))
    {
        fixAfterInsert(node);
    }
}
void remove(const T &val)
{
    if(root_==nullptr)
    {
        return;
    }
    Node*cur=root_;
    while(cur!=nullptr)
    {
        if(cur->data_>val)
        {
            cur=cur->left_;
        }
        else if(cur->data_<val)
        {
            cur=cur->right_;
        }
        else
        {
            break;
        }
    }
    //没找到
    if(cur==nullptr)
    {
        return;
    }
    //情况3
    if(cur->left_!=nullptr&&cur->right_!=nullptr)
    {
        Node*pre=cur->left_;
        while(pre->right_!=nullptr)
        {
            pre=pre->right_;
        }
        cur->data_=pre->data_;
        cur=pre;//cur指向前驱节点
    }
//删除cur指向的节点   情况1  2
Node*child=cur->left_;
if(child==nullptr)
{
    child=cur->right_;
}
if(child!=nullptr)//有孩子节点
{
    child->parent_=cur->parent_;
    if(cur->parent_==nullptr)
    {
        root_=child;
    }
    else{
        if(cur->parent_->left_==cur)
        {
            cur->parent_->left_=child;
        }
        else
        {
            cur->parent_->right_=child;
        }
    }
    Color c=color(cur);
    delete cur;
    if(c==BLACK)
    {
        fixAfterRemove(child);
    }
}
else//无孩子节点
{
    if(cur->parent_==nullptr)
    {
        delete cur;
        root_=nullptr;
    }
    else
    {
        //删除的cur是叶子节点
        if(color(cur)==BLACK)
        {
            fixAfterRemove(cur);
        }
        if(cur->parent_->left_==cur)
        {
            cur->parent_->left_=nullptr;
        }
        else
        {
            cur->parent_->right_=nullptr;
        }
        delete cur;
    }
}
}
private:
//颜色
enum Color{
BLACK,
RED
};
struct Node{
    Node(T data=T(),Node*parent=nullptr,Node*left=nullptr,Node*right=nullptr,Color color=BLACK):
    data_(data),left_(left),right_(right),parent_(parent),color_(color)
    {}
    T data_;
    Node*left_;
    Node*right_;
    Node*parent_;
    Color color_;
};
Color color(Node*node)//返回节点颜色
{
    return node==nullptr ? BLACK:node->color_;
}
void setColor(Node*node,Color color)
{
    node->color_=color;
}
Node*left(Node*node)
{
    return node->left_;
}
Node*right(Node*node)
{
    return node->right_;
}
Node*parent(Node*node)
{
    return node->parent_;
}
//左旋转
void leftRotate(Node*node){
    Node*child=node->right_;
    child->parent_=node->parent_;
    if(node->parent_==nullptr)
    {
        //node是根节点
        root_=child;
    }
    else
    {
        if(node->parent_->left_==node)
        {
            node->parent_->left_=child;
        }
        else
        {
            node->parent_->right_=child;
        }
    }
    node->right_=child->left_;
    if(child->left_!=nullptr)
    {
        child->left_->parent_=node;
    }
    child->left_=node; 
    node->parent_=child;
}
//右旋转
void rightRotate(Node*node){
    Node*child=node->left_;
    child->parent_=node->parent_;
    if(node->parent_==nullptr)
    {
        //node是根节点
        root_=child;
    }
    else
    {
        if(node->parent_->left_==node)
        {
            node->parent_->left_=child;
        }
        else
        {
            node->parent_->right_=child;
        }
    }
    node->left_=child->right_;
    if(child->right_!=nullptr)
    {
        child->right_->parent_=node;
    }
    child->right_=node; 
    node->parent_=child;
}
//红黑树的插入调整
void fixAfterInsert(Node*node)
{
    //如果当前节点的父节点也是红色，继续
    while(color(parent(node))==RED)
    {
        if(left(parent(parent(node)))==parent(node))
        {
            Node*uncle=right(parent(parent(node)));
            if(RED==color(uncle))//情况一，叔叔节点也是红色
            {
                setColor(parent(node),BLACK);
                setColor(uncle,BLACK);
                setColor(parent(parent(node)),RED);
                node=parent(parent(node));//类似递归，继续向上判断
            }
            else
            {
                //先处理情况3
                if(right(parent(node))==node)
                {
                    node=parent(node);
                    leftRotate(node);
                }
                //然后情况2
                setColor(parent(node),BLACK);
                setColor(parent(parent(node)),RED);
                rightRotate(parent(parent(node)));
                break;//调整完成
            }
        }
        else
        {
            Node*uncle=left(parent(parent(node)));
            if(RED==color(uncle))//情况一，叔叔节点也是红色
            {
                setColor(parent(node),BLACK);
                setColor(uncle,BLACK);
                setColor(parent(parent(node)),RED);
                node=parent(parent(node));//类似递归，继续向上判断
            }
            else
            {
                //先处理情况3
                if(left(parent(node))==node)
                {
                    node=parent(node);
                    rightRotate(node);
                }
                //然后情况2
                setColor(parent(node),BLACK);
                setColor(parent(parent(node)),RED);
                leftRotate(parent(parent(node)));
                break;//调整完成
            }
        }
    }
    //强制根节点为黑色
    setColor(root_,BLACK);
}
//红黑树的删除调整
void fixAfterRemove(Node*node)
{
    while(color(node)==BLACK)
    {
         if(left(parent(node))==node)
         {
            //删除的黑色节点在左子树
            Node*brother=right(parent(node));
            if(color(brother)==RED)//情况4
            {
                setColor(parent(node),RED);
                setColor(brother,BLACK);
                leftRotate(parent(node));
                brother=right(parent(node));
            }
            if(color(brother->left_)==BLACK&&color(brother->right_)==BLACK)
            {
                //都是黑色，情况3
                setColor(brother,RED);
                node=parent(node);
            }
            else
            {
                if(color(right(brother))!=RED)//情况2
                {
                    setColor(brother,RED);
                    setColor(left(brother),BLACK);
                    rightRotate(brother);
                    brother=right(parent(node));
                }
                //最后都到情况1
                setColor(brother,color(parent(node)));
                setColor(parent(node),BLACK);
                setColor(right(brother),BLACK);
                leftRotate(parent(node));
                break;
            }
         }
         else
         {
            //删除的黑色节点在右子树
            Node*brother=left(parent(node));
            if(color(brother)==RED)//情况4
            {
                setColor(parent(node),RED);
                setColor(brother,BLACK);
                rightRotate(parent(node));
                brother=left(parent(node));
            }
            if(color(brother->left_)==BLACK&&color(brother->right_)==BLACK)
            {
                //都是黑色，情况3
                setColor(brother,RED);
                node=parent(node);
            }
            else
            {
                if(color(left(brother))!=RED)//情况2
                {
                    setColor(brother,RED);
                    setColor(right(brother),BLACK);
                    leftRotate(brother);
                    brother=left(parent(node));
                }
                //最后都到情况1
                setColor(brother,color(parent(node)));
                setColor(parent(node),BLACK);
                setColor(left(brother),BLACK);
                rightRotate(parent(node));
                break;
            }
         }
    }
    //直接调整孩子节点成红色，结束
    setColor(node,BLACK);
}


Node*root_;

};











int main(){
  RBTree<int>rb;
  for(int i=1;i<=10;i++){
    rb.insert(i);
  }
rb.remove(9);
cout<<"hello"<<endl;
}
    
