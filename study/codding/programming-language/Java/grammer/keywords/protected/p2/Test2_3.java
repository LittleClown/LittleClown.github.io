/*~
 *  Created by lemon-clown on 2017/9/11
 */
package localtests.protectedtest.p2;

import localtests.protectedtest.p1.MyObject1_3;

class MyObject2_3 extends MyObject1_3 {}

public class Test2_3 extends MyObject1_3 {
    void test() {
        MyObject1_3 myObject1_3 = new MyObject1_3();
//        myObject1_3.clone();
    }

    public static void main(String[] args) throws CloneNotSupportedException {
        MyObject1_3 myObject1_3 = new MyObject1_3();
//        myObject1_3.clone();

        MyObject2_3 myObject2_3 = new MyObject2_3();
//        myObject2_3.clone();

        Test2_3 test2_3 = new Test2_3();
        test2_3.clone();
    }
}
