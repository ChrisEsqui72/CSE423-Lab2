#shebang to be ignored
// inline comment \n \r
fun main() {
    val numbers = listOf(1.54, 2, 0.3, 04, 5)
    var sum = 0

    for (n in numbers) {
        if (n == 3) {
            continue
        }

        sum += n

        if (sum > 6) {
            break
        }
    }

    val result = when (sum) {
        0 -> "zero"
        (int)1, 2, 3 -> "small"
        else -> "large"
    }

    val flag = true
    val otherFlag = false

    if (flag && !otherFlag) {
        do {
            println("Sum is \n $sum and result is $result")
            return
        } while (false)
    } else {
        val nothing: String? = null
        println(nothing)
    }
}
