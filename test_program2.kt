fun testOps(p: Int, q: Int?): Int {
    var a: Int = p
    val b: Int? = q

    // nullable type operator token
    val maybeNum: Int? = null

    // safe call, elvis, and non-null assert
    val temp = b?.toString() ?: "default"
    // val crash = b!!.toString()  // token test only

    // boolean logic
    val logic = (a > 0) && (a < 100) || !(a == 50)

    // arithmetic
    a = a + 5
    a = a - 5
    a = a * 2
    a = a / 2
    a = a % 2

    // += and -=
    a += 10
    a -= 3

    // comparisons
    val g1 = a >= 0
    val g2 = a <= 999
    val g3 = a > 5
    val g4 = a < 500
    val g5 = a == 10
    val g6 = a != 11

    // referential comparisons
    val s1: Any = "abc"
    val s2: Any = "abc"
    val r1 = (s1 === s2)
    val r2 = (s1 !== s2)

    // array access
    val nums = intArrayOf(9, 8, 7)
    val n0 = nums[0]

    // dot operator
    val len = casted.length

    // identifiers (stress test)
    val IDENTIFIER_TEST_123 = 42
    var anotherIdentifier = IDENTIFIER_TEST_123

    // parentheses and braces tested naturally
    if (anotherIdentifier > 0) {
        anotherIdentifier--
    } else {
        anotherIdentifier++
    }

    return a
}

fun main() {
    val result = testOps(10, null)
    println(result)
}
