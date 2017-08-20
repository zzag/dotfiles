import Data.List (insert, unfoldr, transpose)


-- bubble sort
bubbleSort :: (Ord a) => [a] -> [a]
bubbleSort [] = []
bubbleSort [x] = [x]
bubbleSort x = 
  (iterate swapPass x) !! ((length x) - 1)
  where swapPass [x] = [x]
        swapPass (x1:x2:xs)
          | x1 > x2 = x2 : swapPass (x1:xs)
          | otherwise = x1 : swapPass (x2:xs)


-- insertion sort
insertionSort :: (Ord a) => [a] -> [a]
insertionSort [] = []
insertionSort [x] = [x]
insertionSort (x:xs) = 
  insert $ insertionSort xs
  where insert [] = [x]
        insert (y:ys)
          | x < y = x : y : ys
          | otherwise = y : insert ys


-- cocktail sort
cocktailSort :: (Ord a) => [a] -> [a]
cocktailSort [] = []
cocktailSort [x] = [x]
cocktailSort x =
  [head pass] ++ cocktailSort middle ++ [last pass]
  where pass = swapPass (<) $ reverse $ swapPass (>) $ reverse x
        middle = drop 1 $ reverse $ drop 1 $ reverse pass
        swapPass :: (a -> a -> Bool) -> [a] -> [a]
        swapPass _ [] = []
        swapPass _ [x] = [x]
        swapPass f (x1:x2:xs)
          | f x1 x2 = x1 : swapPass f (x2:xs)
          | otherwise = x2 : swapPass f (x1:xs)


-- quick sort
quickSort :: (Ord a) => [a] -> [a]
quickSort [] = []
quickSort [x] = [x]
quickSort (x:xs) =
  let smaller = quickSort [y | y <- xs, y <= x]
      bigger = quickSort [y | y <- xs, y > x]
  in smaller ++ [x] ++ bigger


-- shell sort
shellSort :: (Ord a) => [a] -> [a]
shellSort [] = []
shellSort [x] = [x]
shellSort xs =
  foldr epoch xs gaps
  where gaps = takeWhile (< length xs) $ iterate (2*) 1
        epoch gap = concat .                    -- [1, 2, 3, 4, 5]
                    transpose .                 -- [[1, 2], [3, 4], [5]]
                    map (foldr insert []) .     -- sort
                    transpose .                 -- [[1, 3, 5], [2, 4]]
                    takeWhile (not . null) .    -- [[1, 2], [3, 4], [5]]
                    unfoldr(Just . splitAt gap) -- [[1, 2], [3, 4], [5], [], [], []]
