//: Playground - noun: a place where people can play

import UIKit

func removeDigitsFromCity(string: NSString) -> NSString {
    var returnString = string
    var decRange = string.rangeOfCharacterFromSet(NSCharacterSet.decimalDigitCharacterSet())
    
    while decRange.location != NSNotFound{
        print("Range.location \(decRange.location) Range.length \(decRange.location)")
        returnString = returnString.stringByReplacingCharactersInRange(decRange, withString: "")
        print("returnString: " + (returnString as String))
        decRange = returnString.rangeOfCharacterFromSet(NSCharacterSet.decimalDigitCharacterSet())
    }
    
    return returnString
}

removeDigitsFromCity("")
