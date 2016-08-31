# Authorization Modulle
Discussion about this was done [here](https://basecamp.com/2094388/projects/6775356/messages/34525548)
## Discarded Implementation
An AM (Authorization Module) file (a plain text file with extension .am) is provided for an app if it needs to make API calls. The name of AM file should match the QM file name. For example, the AM file for alertness.qm should be alertness.am. 

An app can make an API call if it has the API listed in the AM file as one of the two methods.

* < API-Name>
* < API-Name> < TAB> < Mandatory-First-Argument>