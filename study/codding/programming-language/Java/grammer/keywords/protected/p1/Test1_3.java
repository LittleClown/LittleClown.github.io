/*~
 *  Created by lemon-clown on 2017/9/11
 */
package localtests.protectedtest.p1;

public class Test1_3 extends MyObject1_3 {
    void test() throws CloneNotSupportedException {
        MyObject1_3 myObject1_3 = new MyObject1_3();
        myObject1_3.clone();
    }

    public static void main(String[] args) throws CloneNotSupportedException {
        MyObject1_3 myObject1_3 = new MyObject1_3();
        myObject1_3.clone();

        Test1_3 test1_3 = new Test1_3();
        test1_3.clone();
    }
}
