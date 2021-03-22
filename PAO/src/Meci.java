package src;

import java.util.Scanner;

public class Meci {
    public int scorHome, scorAway;
    public String nameHome, nameAway;

    public static Meci read(Scanner scan) {
        Meci m = new Meci();
        m.nameHome = scan.next();
        m.scorHome = scan.nextInt();
        scan.next();
        m.scorAway = scan.nextInt();
        m.nameAway = scan.next();
        return m;
    }
}
