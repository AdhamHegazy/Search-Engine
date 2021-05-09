# Search-Engine
A search engine  built using c++ (vectors, maps and classes were used to maintain a practical complexity), that sorts the pages based on the pagerank.

1) Pseudo code:
● First i open the files of impressions, in which i use a constructor to the struct webpage.
● This constructor receives the string (which is the link)
● I then input the impressions into the struct
● Also i link this struct with a map, such that the map links the string of link to make it easy
to search for the specific struct of each website using only the link
● I then start inputting the keywords, i put it in a map of strings such that each keyword is
mapped to a vector of strings, which are the links of websites that include this keyword
● This map would allow the keyword to be searched for in almost a o(1) time complexity.
● The results of the searched keyword is sorted based on their score:
  ○ I sort the score, and calculate this using the rank and ctr, where the ctr is
  clicks/impressions
  score = 0.4 * (normalized page rank) + ((1- (0.1* impressions) / (1+0.1*impressions)) * (normalized page rank) + (0.1* impressions) / (1+0.1*impressions) * CTR) * 0.6   
  
● The user can then choose to open a webpage, which will result in clicks++
● The page rank is calculated using 3 iterations, where the first iteration is 1/number of
webpages, the second and third are based on the webpages pointing to the webpage im
standing in.
● Note: you need to change the directory of the files to make them open

2) Time complexity O(n^3)
Space complexity, I don't use extra space so O(1)

3)I used a lot of vectors in my project due to the simplicity and the flexibility of design. I used
maps too to help with linking websites and the class of webpages.
