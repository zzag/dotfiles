import Data.List


split1 xs k = (takeWhile (not . null) . unfoldr (Just . splitAt k)) xs
split2 xs k = takeWhile (not . null) $ unfoldr (\x -> Just(splitAt k x)) xs
