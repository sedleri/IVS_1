//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

using namespace ::testing;

class EmptyTree : public Test
{
  virtual void SetUp(){
    tree = new BinaryTree();
  }
protected:
  BinaryTree *tree;
};

class NonEmptyTree : public Test
{
  virtual void SetUp()
  {
    tree = new BinaryTree();
    for (int i = 0; i < 4; i++)
    {
      tree->InsertNode(i);
    }
  }

  virtual void TearDown()
  {
    delete tree;
  }

protected:
  BinaryTree *tree;
};

class TreeAxioms : public Test
{
  virtual void SetUp()
  {
    tree = new BinaryTree();
    tree2 = new BinaryTree();

    for(int i = 0; i < 3; i++){
      tree2->InsertNode(i);
    }
  }

  virtual void TearDown()
  {
    delete tree, tree2;
  }

protected:
  BinaryTree *tree, *tree2;
};

TEST_F(EmptyTree, FindNode)
{
  ASSERT_EQ(NULL, tree->FindNode(1));
  ASSERT_EQ(NULL, tree->FindNode(10));
}

TEST_F(EmptyTree, InsertNode)
{
  for (int i = 0; i < 5; i++)
  {
    tree->InsertNode(i);
  }

  ASSERT_TRUE(tree->FindNode(4) != NULL);
  ASSERT_TRUE(tree->FindNode(1) != NULL);
  ASSERT_TRUE(tree->FindNode(2) != NULL);
  ASSERT_TRUE(tree->FindNode(5) == NULL);
}

TEST_F(EmptyTree, DeleteNode)
{
  tree->InsertNode(1);
  tree->InsertNode(2);
  tree->DeleteNode(1);
  ASSERT_TRUE(tree->FindNode(1) == NULL);
  ASSERT_TRUE(tree->FindNode(2) != NULL);
}

TEST_F(NonEmptyTree, FindNode)
{
  ASSERT_TRUE(tree->FindNode(1) != NULL);
  ASSERT_TRUE(tree->FindNode(3) != NULL);
  ASSERT_EQ(NULL, tree->FindNode(4));
}

TEST_F(NonEmptyTree, InsertNode)
{
  ASSERT_EQ(false, tree->InsertNode(1).first);
  ASSERT_EQ(false, tree->InsertNode(2).first);
  ASSERT_EQ(true, tree->InsertNode(4).first);
  ASSERT_EQ(tree->FindNode(1), tree->InsertNode(1).second);
  ASSERT_EQ(tree->FindNode(2), tree->InsertNode(2).second);
}

TEST_F(NonEmptyTree, DeleteNode)
{
  ASSERT_EQ(true, tree->DeleteNode(1));
  ASSERT_EQ(false, tree->DeleteNode(654));
}

TEST_F(TreeAxioms, Axiom1)
{
  std::vector<Node_t *> leaf;
  tree2->GetLeafNodes(leaf);
  for(int i = 0; i < leaf.size(); i++){
    ASSERT_EQ(BLACK, leaf[i]->color);
  }
}

TEST_F(TreeAxioms, Axiom2)
{
  std::vector<Node_t *> nonLeafNode;
  tree2->GetNonLeafNodes(nonLeafNode);
  for(int i = 0; i < nonLeafNode.size(); i++){
    if(nonLeafNode[i]->color == RED){
      ASSERT_EQ(nonLeafNode[i]->pLeft->color, BLACK);
      ASSERT_EQ(nonLeafNode[i]->pRight->color, BLACK);
      ASSERT_EQ(nonLeafNode[i]->pParent->color, BLACK);
    }else{
      ASSERT_EQ(nonLeafNode[i]->color, BLACK);
    }
  }
}

TEST_F(TreeAxioms, Axiom3)
{
  std::vector<BinaryTree::Node_t *> leaf;
  Node_t *nodes;
  tree2->GetLeafNodes(leaf);
  int blackCount = 0, expectedCount = -1;

  for(Node_t *node : leaf){
      blackCount = 0;
      nodes = node;
      while(nodes){
          if(nodes->color == BLACK){
              blackCount++;
          }
          nodes = nodes->pParent;
      }
    if(expectedCount != -1){
        ASSERT_EQ(expectedCount, blackCount);
    }
    expectedCount = blackCount;
  }
}

/*** Konec souboru black_box_tests.cpp ***/