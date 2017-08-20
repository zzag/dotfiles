data Tree a = Empty | Node a (Tree a) (Tree a) deriving (Eq, Show, Read)

-- insert in tree
treeInsert :: (Ord a) => a -> Tree a -> Tree a
treeInsert x Empty = Node x Empty Empty
treeInsert x (Node y left right)
  | x == y = Node x left right
  | x < y = Node y (treeInsert x left) right
  | otherwise = Node y left (treeInsert x right)


-- contains an element
treeHas :: (Ord a) => a -> Tree a -> Bool
treeHas x Empty = False
treeHas x (Node y left right)
  | x == y = True
  | x < y = treeHas x left
  | otherwise = treeHas x right

-- delete from a tree
treeDelete :: (Ord a) => a -> Tree a -> Tree a
treeDelete x Empty = Empty
treeDelete x (Node y left right)
  | x == y = delete (Node y left right)
  | x < y = Node y (treeDelete x left) right
  | otherwise = Node y left (treeDelete x right)
  where delete :: (Ord a) => Tree a -> Tree a
        delete (Node _ l Empty) = l
        delete (Node _ Empty r) = r
        delete (Node _ l r) = let succ = successor l
                              in (Node succ (treeDelete succ l) r)

        successor :: (Ord a) => Tree a -> a
        successor (Node x l r)
          | r == Empty = x
          | otherwise = successor r
