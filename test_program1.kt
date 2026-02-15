import kotlin.math.*

const val CONST_NUM: Int = 5

fun main() {
    val x: Int = 10
    var y: Int = 20
    var z: Int? = null

    val truth: Boolean = true
    val lie: Boolean = false

    if (truth && !lie || x > 5) {
        y += 1
    } else {
        y -= 1
    }

    // while loop
    var i: Int = 0
    while (i < 3) {
        i++
        if (i == 2) {
            continue
        }
        if (i >= 3) {
            break
        }
    }

    // do while loop
    var j: Int = 0
    do {
        j++
    } while (j <= 2)

    // for loop with range inclusive
    for (k in 1..3) {
        println(k)
    }

    // for loop with range exclusive (until)
    for (m in 1 until 3) {
        println(m)
    }

    // when statement
    when (x) {
        1 -> println("one")
        2 -> println("two")
        else -> println("other")
    }

    // null check + elvis operator
    val safeVal: Int = z ?: 99

    // safe call + non-null assert
    val length1 = z?.toString()?.length
    // val length2 = z!!.toString().length  // will crash if executed, but tests token

    // assignment operators
    y = y + 1
    y = y - 1
    y = y * 2
    y = y / 2
    y = y % 3

    // inc/dec
    y++
    y--

    // comparisons
    val a = (x >= y)
    val b = (x <= y)
    val c = (x > y)
    val d = (x < y)
    val e = (x == y)
    val f = (x != y)

    // referential equality
    val obj1: Any = "hello"
    val obj2: Any = "hello"
    val refEq = (obj1 === obj2)
    val refNeq = (obj1 !== obj2)

    // array access
    val arr = arrayOf(1, 2, 3)
    val arrVal = arr[0]

    // dot operator
    val dotTest = casted.length

    // literals
    val intLit = 12345
    val floatLit = 3.14159
    val charLit = 'A'
    val stringLit = "compiler test"

    return
}
