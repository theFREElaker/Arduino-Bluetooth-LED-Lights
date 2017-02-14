//: Playground - noun: a place where people can play

import UIKit

var str = "Hello"

var newstr = str + " Bob"

for characters in newstr.characters{
    print(characters)
}

var newTypeString = NSString(string: newstr)

newTypeString.substringToIndex(5)
