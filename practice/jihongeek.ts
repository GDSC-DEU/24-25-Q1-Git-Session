interface Person{
    name : string,
    age : number
}

const jihong:Person = {
    name : "Jihong Kim",
    age : 24
} 

const greeting = `Hello, My name is ${jihong.name} and I'm ${jihong.age} years old. I'm fine thank you and you?`
console.log(greeting);