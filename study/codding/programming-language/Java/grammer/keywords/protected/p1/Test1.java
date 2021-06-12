/*~
 *  Created by lemon-clown on 2017/9/11
 */
package localtests.protectedtest.p1;

import localtests.protectedtest.p2.Son2;

public class Test1 {
    public static void main(String[] args) {
        Son1 son1 = new Son1();
        son1.f();

        Son2 son2 = new Son2();
        son2.f();
    }
}
