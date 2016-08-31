Open Source Release -- TODO 
=======
We need to do all this (fill in more items) before presenting to the shepherd.


- Fill in all of the missing information in ATTRIBUTIONS.txt
- Figure out and add our own license to LICENSE.txt
- [Here’s how we should create a DOI and link for our September 1 release of the Amulet code](https://guides.github.com/activities/citable-code/)
- [Here's how we should add copyright headers to all of our code](https://github.com/osterman/copyright-header)
	- Installation note: I was missing a couple of packages, which caused my install to fail.

			brew install icu4c
			brew install cmake

	- Also, installing the ri documentation was taking FOREVER, so I went ahead and used this command to skip the documentation.

			sudo gem install copyright-header --no-ri --no-rdoc
