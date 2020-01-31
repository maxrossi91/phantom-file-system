# Proof of concept



Create the database of the `folder-of-intrest` using
`
updatedb -l 0 -o ~/.externalharddisk.db -U folder-of-intrest
`

Search the `searchterm` using
`
locate -d ~/.externalharddisk.db: searchterm
`

### Example

`
updatedb -l 0 -o ~/.externalharddisk.db -U /media/user/*/
`

`
locate -d ~/.externalharddisk.db: main.cpp
`

### 