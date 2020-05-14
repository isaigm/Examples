module Stuff where
import Data.Bits
import Data.Foldable
quicksort :: [Int] -> [Int]
quicksort [] = []
quicksort (x:xs) = [y | y <- xs, y < x] ++ [x] ++ [z | z <- xs, z >= x]

maxInList :: [Int] -> Int
maxInList [] = error "Empty list"
maxInList [x] = x
maxInList (x:xs) = max2 x (maxInList xs) where
    max2 :: Int -> Int -> Int
    max2 a b | a > b = a | otherwise = b

toBits :: Int -> String
toBits k
    | k == 0 = "0"
    | k == 1 = "1"
    | k > 0 = toBits (shiftR k 1) ++ show ((.&.) k 1)
    | otherwise = ""

getNumAt :: [Int] -> Int -> Int
getNumAt [] _ = error "Empty List"
getNumAt xs k = xs !! k

myLength :: [a] -> Int
myLength [] = 0
myLength (x:xs) = 1 + myLength xs 

myReverse :: [a] -> [a]
myReverse [] = []
myReverse xs = last xs:myReverse(take (length xs - 1) xs)

curry1 :: (a -> a) -> a ->  a
curry1 f x = f (f x)

myButLast :: [a] -> a
myButLast [] = error "Empty List"
myButLast [x] = error "No last but one element"
myButLast [x, y] = x
myButLast (x:xs) = myButLast xs

myMap :: (a -> b) -> [a] -> [b]
myMap _ [] = []
myMap f (x:xs) = f x : (myMap f xs)

collatz :: Int -> [Int]
collatz 1 = [1]
collatz n
    | mod n 2 == 0 = n:collatz (div n 2)
    | otherwise = n:collatz (3*n + 1)

numLongChains :: Int
numLongChains = length (filter isLong (map collatz [1..100]))
    where isLong xs = length xs > 15

plus :: Int -> Int
plus x = let plusOne y = y + 1 
                     in plusOne x
xorForList :: [Int] -> Int
xorForList = foldl xor 0