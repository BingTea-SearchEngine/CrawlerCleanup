# Programs for cleaning up crawler output

## Detect + Delete duplicate urls
Some urls have been crawled and parsed multiple time. I need some program to detect these duplicates and delete them if needed.

### Input
- Path to a directory of files in the format x.parsed where x is a number (directory also includes a logs.txt file but can be ignored)
- Options
    - -d: Delete documents with urls that we have already seen before (should make sure each url appears once)

The x.parsed file looks like this:
```
URL: https://en.wikipedia.org/wiki/Special:Random Doc number: 0
<title>
...
</title>
<words>
...
</words>
<links>
...
</links>
```
Get the url of a document by parsing in between `URL: ` and `Doc`

### Output
The program should output a url to document name mapping where a url may contain more than one document
```
https://en.wikipedia.org/wiki/Special:Random:
    -> 0.parsed
    -> 5.parsed
    -> 1311.parsed
https://google.com
    -> 1.parsed
https://reddit.com
    -> 2.parsed
    -> 12.parsed
```

If the `-d` flag is enabled, the output should also contain
```
Original document count: x
Documents removed: y
Final document count: x-y
```

## Domain finder + Delete
There are a lot of sites that we wouldn't want in our index. We want to find these sites and how many times it occurs and potentially delete x.parsed files that contain these urls.

### Input
- Base url of a site. For example, wikipedia links look like https://en.wikipedia.org/wiki/Colonization_of_the_Congo_Basin but the base url is https://en.wikipedia.org/. If I want to find all sites that belong to wikipedia, I will pass in https://en.wikipedia.org/
- Options
    - -d: Delete documents that contain urls where the base url matches with the input

### Output
A list of url and document with the base url and the number of documents that match it.
```
https://en.wikipedia.org/wiki/hi: 0.parsed
https://en.wikipedia.org/wiki/no: 15.parsed
https://en.wikipedia.org/wiki/because: 231.parsed
...
Number of document matches: x
```
If the `-d` flag is enabled, the output should also contain
```
Original document count: x
Documents removed: y
Final document count: x-y
```