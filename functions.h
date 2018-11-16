#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <fstream>
#include <string>
#include "struct.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                          ITERATIVE REMOVAL                               //
/////////////////////////////////////////////////////////////////////////////

//left rotate x and x.right
void Delete_Left_Rotate(struct node *x){
    node *y;
    y = x->right;
    x->right = y->left;
    if (y->left != nill)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nill)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    y->left = x;
    x->parent  = y;
}

//right rotate y and y.left
void Delete_Right_Rotate(struct node *y){
    node *x;
    x = y->left;
    y->left = x->right;
    if (x->right != nill)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nill)
        root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    x->right = y;
    y->parent  = x;
}


///////////////////////////////////////////////////////////////////////////////
//replace the subtree rooted at node aux with the subtree rooted at aux-left or aux->right
void RB_transplant(struct node *aux, struct node *auxchild){
    if (aux->parent == nill)             //if aux=root child becomes root
        root = auxchild;
    else if (aux == aux->parent->left)     //if child is a left
        aux->parent->left = auxchild;      //connect grandparentarent's->left with child
    else
    	aux->parent->right = auxchild;     //if child is right connect grandparentarent's->right with child
    auxchild->parent = aux->parent;	   //connect child to parentoint to parentarent
}


//find the successor of a node
node *tree_successor(struct node *p){
    while (p->left != nill)
        p = p->left;
    return p;
}


///////////////////////////////////////////////////////////////////////////////
//fix the balance after a node deletion
void RB_delete_fix(struct node *x, struct node *w){
    while (x != root && x->color == BLACK) {
        //x is left
        if (x == x->parent->left){
            w = x->parent->right;               //w is x's sibling
            //case 1: x's sibling w is RED, w must have BLACK children
            //we switch colors of w <=> x.parent and then Left_Rotation
            if (w->color == RED){
                w->color    = BLACK;    //case 1 is converted into case 2,3 or 4.
                x->parent->color = RED;
                Delete_Left_Rotate(x->parent);      //x->parent with w
                w = x->parent->right;
            }
            //case 2: x's sibling w is BLACK, and both of w's children are BLACK to
            //comparentensate for removing 1 BLACK we add extra BLACK to x.parent which and
            //we do so by reparenteating (while looparent) with x.parent as new node x was RED or BLACK
	    //we remove 1 BLACK color from x and w
            else if (w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            //case 3:x's sibling w is BLACK, w's left is RED and w's right is BLACK
            //switch colors w <=> w.left and parenterform Right_Rotation on w without
            //violations of RBT
            else if (w->right->color == BLACK){
                w->left->color = BLACK;	//new sibling w of x is now BLACK with RED right
                w->color     = RED;	//thus case 3 transformed to case 4
                Delete_Right_Rotate(w);
                w = x->parent->right;
            }
            //case 4: x's sibling w is BLACK and w's right is RED
            //color changes and Left_Rotation on x.parent we can remove the extra BLACK on x
            //making it singly BLACK. Setting x=root we terminate the while looparent
            else{
                w->color     = x->parent->color;
                x->parent->color  = BLACK;
                w->right->color = BLACK;
                Delete_Left_Rotate(x->parent);
                x = root;               //for termination of looparent
            }
        }
        //x is right
        else{
            w = x->parent->left;
            if (w->color == RED){
                w->color    = BLACK;
                x->parent->color = RED;
                Delete_Right_Rotate(x->parent);
                w = x->parent->left;
            }
            else if (w->right->color == BLACK && w->left->color == BLACK){
                w->color=RED;
                x=x->parent;
            }
            else if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color     = RED;
                Delete_Left_Rotate(w);
                w = x->parent->left;
            }
            else{
                w->color     = x->parent->color;
                x->parent->color  = BLACK;
                w->left->color = BLACK;
                Delete_Right_Rotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}


//delete z node, if z has fewer than 2 children we remove it and reparentlace
//it with successor(always successor->left=nill)
void RBDelete(struct node* z, struct node* y, struct node *x){
    enum type originacolor;	//keeparent track of x which moves into y's original parentosition
    originacolor = y->color;	//Keeparent track of original color

    //case 1: z has no left
    if (z->left == nill){
        x = z->right;
        RB_transplant(z, z->right);    //parentointers of z's R child and z's parent parentoint each other
    }
    //case 2: z has left but no right
    else if (z->right == nill){
        x = z->left;
        RB_transplant(z, z->left);
    }
    //two cases: z has both Children
    else{
        y = tree_successor(z->right);      //find successor
        originacolor = y->color;       //save color of successor
        x=y->right;
        //successor has no left=>nill (its the minimum to the left)
        if (y->parent == z){
            x->parent = y;
        }
        //swaparent subtree of y->right parentointing to y->parent (before we move y to z)
        else {
            RB_transplant(y, y->right);
            y->right    = z->right;		//parentartial change of y
            y->right->parent = y;
        }
        //reparentlacement of z with y (also builds subtrees)
        RB_transplant(z, y);
        y->left    = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    //imbalanced RBT only parentossible when we delete a BLACK node
    if (originacolor == BLACK)
        RB_delete_fix(x,x);
    free(z);
}




//////////////////////////////////////////////////////////////////////////////
//                          RECURSIVE INSERTION                              //
/////////////////////////////////////////////////////////////////////////////

void insert_Left_Rotate(node *&p){
    node *son, *temp, *grandparent;
    grandparent = p->parent;
    son = p->right;
    temp = son->left;

    p->right = temp;
    if(temp!=nill)
        temp->parent = p;
    son->parent = grandparent;

    son->left = p;
    p->parent = son;
    p = son;
}


/*
SE O TIO É parentRETO E SE O parentAI E O FILHO ESTÃO EM LINHA INCLINADA A ESQUERDA, OU SEJA,
QUANDO O NODO  INSERIDO A ESQUERDA DO parentAI E O TIO ESTÁ A DIREITA DO parentAI
*/
void insert_Right_Rotate(node *&p){
    node *son, *temp, *grandparent;
    grandparent = p->parent;
    son = p->left;
    temp = son->right;
    p->left = temp;

    if(temp!=nill)
        temp->parent = p;
    son->parent = grandparent;

    son->right = p;
    p->parent = son;
    p = son;
}


//NODO INSERIDO A DIREITA DO PAI E O TIO PRETO ESTA NA ESQUERDA
void insert_Double_Left_Right_Rotate(node * &p){

    insert_Left_Rotate(p->left);
    p->left->color = BLACK;
    p->left->left->color=RED;

    insert_Right_Rotate(p);
    p->color = BLACK;
    p->right->color=RED;
}


//NODO INSERIDO A ESQUERDA DO PAI E TIO PRETO ESTA NA DIREITA
void insert_Double_Right_Left_Rotate(node * &p){

    insert_Right_Rotate(p->right);
    p->right->color = BLACK;
    p->right->right->color=RED;

    insert_Left_Rotate(p);
    p->color = BLACK;
    p->left->color=RED;
}


//CORRECAO DA RUBRO NEGRA
void insert_RB_Fixup(node * &p){
    node * leftSon = p->left;
    node * rightSon  = p->right;
    //FILHO DA ESQUERDA: parentAI É VERVELHO E TIO TBM É VERMELHO
    if(leftSon != nill && leftSon->color == RED){
        node * leftGrandSon = leftSon->left;
        node * rightGrandSon = leftSon ->right;
        //TIO DO NODO INSERIDO EH VERMELHO
        if(rightSon!= nill && rightSon->color==RED){
            if((leftGrandSon!= nill && leftGrandSon->color==RED)||(rightGrandSon!= nill && rightGrandSon->color==RED)){
            //TROCAR AS CORES DO PAI, TIO E AVO
                //cout<<"AVO:"<<p->key<<" - parentAI:"<<leftSon->key <<" - TIO:"<<rightSon->key<<"\n";
                p->color = RED;
                rightSon->color = BLACK;
                leftSon->color = BLACK;
                //cout<<"PAI, TIO E AVO FORAM TROCADOS DE COR!\n\n\n";
            }
            // PULAR PARA O SEGUNDO TESTE:
            else
                goto second_test;
        }
        // SE O FILHO DA DIREITA NAO É VERMELHO, ELE É parentRETO OU NILL
        else{
            if(leftGrandSon!= nill && leftGrandSon->color==RED){
                insert_Right_Rotate(p);
                p->color = BLACK;
                p->right->color=RED;
            }
            else if(rightGrandSon!= nill && rightGrandSon->color==RED){
                insert_Double_Left_Right_Rotate(p);
            }
            else
                goto second_test;
        }
    }
    //FILHO DIREITA: parentAI É VERVELHO E TIO TBM É VERMELHO
    second_test:
    if(rightSon != nill && rightSon->color == RED){
        node * leftGrandSon = rightSon->left;
        node * rightGrandSon = rightSon ->right;
        //TIO DO NODO INSERIDO EH VERMELHO
        if(leftSon!= nill && leftSon->color==RED){
            if((leftGrandSon!= nill && leftGrandSon->color==RED)||(rightGrandSon!= nill && rightGrandSon->color==RED)){
            //TROCAR AS CORES DO PAI, TIO E AVO
                //cout<<"AVO:"<<p->key<<" - PAI:"<<rightSon->key <<" - TIO:"<<leftSon->key<<"\n";
                p->color = RED;
                rightSon->color = BLACK;
                leftSon->color = BLACK;
                //cout<<"PAI, TIO E AVO FORAM TROCADOS DE COR!\n\n\n";
            }
        }
        // SE O FILHO DA DIREITA NAO É VERMELHO, ELE É parentRETO OU NILL
        else{
            if(rightGrandSon!= nill && rightGrandSon->color==RED){
                insert_Left_Rotate(p);
                p->color = BLACK;
                p->left->color=RED;
            }
            else if(leftGrandSon!= NULL && leftGrandSon->color==RED){
                insert_Double_Right_Left_Rotate(p);
            }
        }
    }
}


// RECURSIVE INSERT
void RBInsert(node * &tree, string key){
    if(tree == nill){
        tree = new node;
        tree->key = key;
        tree->color = RED;
        tree->right = nill;
        tree->left = nill;
        tree->parent = nill;
        //cout<<"\n\nINSERTED:"<<tree->key<<"\n";
    }
    else if(key < tree->key){
        RBInsert(tree->left, key);
        //insere o pai
        tree->left->parent = tree;
    }
    else{
        RBInsert(tree->right, key);
        //insere o pai
        tree->right->parent = tree;
    }

    // insert fixup
    insert_RB_Fixup(tree);

    //mantem a propriedade da raíz preta
    if(root != nill && root->color == RED){
        root->color = BLACK;
    }


}




//////////////////////////////////////////////////////////////////////////////
//                          Auxiliary Functions                              //
/////////////////////////////////////////////////////////////////////////////

//search a node in Tree
node * search_node(node * tree, string key){
    if(tree != nill){
        if(tree->key == key)
            return tree;
        else if(key < tree->key)
            return search_node(tree->left, key);
        else
            return search_node(tree->right, key);
    }
    else
        return nill;
}



//check if a node is black
int is_black(node * p){
    if(p->left != NULL && p->right != NULL){
        if(p->left->color == BLACK || p->right->color == BLACK)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}


//Black tree height
int blackHeight(node * root){
    if(root == nill)
        return 0;
    else{
        int leftBlackHeight =  blackHeight(root->left)  + is_black(root);
        int rightBlackHeight = blackHeight(root->right) + is_black(root);

        if(leftBlackHeight == rightBlackHeight)
            return leftBlackHeight;
    }
}

//Black height of a node
int blackHeightNode(string key){
    node * aux = search_node(root, key);
    if(aux != nill){
        return blackHeight(aux);
    }
    else{
        return -1;
    }

}


//create nill and root
void create_Nill_and_Root(){
    nill          = new node;
    nill->color   = BLACK;
    nill->left    = NULL;
    nill->right   = NULL;
    nill->parent  = NULL;
    nill->key     = "";
    root          = nill;
}


//print in order
void RBPrint( node* tree ){
    if( tree != nill){
        RBPrint(tree->left);
        cout<<tree->key<<", ";
        RBPrint(tree->right);
    }
}

//print in pre order
void RBCheck(node * tree){ // pre ordem
    if(tree!=nill){
        cout<<"(";
        if(tree->parent!=nill)
            cout<<tree->parent->key<<", ";
        else
            cout<<"NILL, ";
        cout<<tree->key<<", ";
        if(tree->color==RED)
            cout<<" Vermelho, ";
        else
            cout<<" Preto, ";
        cout<<blackHeightNode(tree->key)<<", ";
        if(tree->left != nill)
            cout<<tree->left->key<<", ";
        else
            cout<<"NILL, ";
        if(tree->right != nill)
            cout<<tree->right->key<<" )\n";
        else
            cout<<"NILL )\n";

        RBCheck(tree->left);
        RBCheck(tree->right);
    }
}

//Checks if the node already exists, if it does not exist, inserts the new node
void insert_rb(string key){
    node * aux = search_node(root, key);
    if(aux == nill){
        RBInsert(root, key);

    }
    else{
        cout<<"Chave "<<key<< " ja existe\n";
    }
}

//Checks if the node already exists, if it does exist, then remove the node
void delete_rb(string key){
    node * aux = search_node(root, key);
    if(aux != nill){
        RBDelete(aux, aux, aux);
        cout<<"Removendo a palavra "<<key<<"\n\n";

        cout<<"Imprimindo em ordem:\n";
        RBPrint(root);
        cout<<"\n\nChecando em pre-ordem:\n";
        RBCheck(root);
    }
    else{
        cout<<"\n\nA palavra "<<key<<" foi removida anteriormente ou nao foi inserida.\n\n";
    }
}

//Takes the data from the file.txt  and inserts / removes it in the tree


#endif // FUNCTIONS_H_INCLUDED
