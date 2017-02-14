//: Playground - noun: a place where people can play

import UIKit
//import Darwin

/*
arc4random_uniform(5)

// Loops

for var i = 2; i <= 20; i = i+2 {
    print(i)
}

for i in 0...10 {
    print(i)
}

for index in 10.stride(to: 0, by: -1){
    print(index)
}

for index in (0...10).reverse(){
    print(index)
}

var array = 0...10
print(array)

for index in array{
    print(index)
}

var array:[Double] = [1, 4, 5, 6, 8]

for (index, value) in array.enumerate(){
    array[index] = value/2
}
print(array)

var mm = 0;
while mm < array.count{
    print(array[mm])
    mm += 1;
}

var arrgh:[Int] = [7, 6 ,5 ,4 ,3 ,2, 1]
mm = 0
while mm < arrgh.count{
    arrgh[mm] -= 1
    print(arrgh[mm])
    mm += 1
}
*/

var number = 2
var isPrime = true

if number > 2{
    for j in 2...(number-1){
        if number % j == 0 {
            isPrime = false
        }
    }
}

print(isPrime)

var kk = 2
isPrime = true
while kk < number{
    if number%kk == 0{
        isPrime = false;
    }
    kk += 1
}

print(isPrime)








