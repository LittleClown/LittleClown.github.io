/*~
 *  Created by lemon-clown on 2017/9/11
 */
package localtests.protectedtest.p1;

import localtests.protectedtest.p2.Test2_5;

public class MyObject1_5 extends Test2_5 {
    @Override
    protected Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
