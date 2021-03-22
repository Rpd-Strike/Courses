package src;

import java.util.Scanner;
import java.util.HashMap;
import java.util.Arrays;

public class Campionat
{
    public static void main( String[] args )
    {
        Scanner scan = new Scanner(System.in);

        int echipe = scan.nextInt();
        int meciuri = scan.nextInt();
        
        Meci[] data = new Meci[meciuri];
        String[] nume_echipe = new String[echipe];
        HashMap<String, Integer> gol_dat = new HashMap<String, Integer>();
        HashMap<String, Integer> gol_primit = new HashMap<String, Integer>();
        HashMap<String, Integer> puncte = new HashMap<String, Integer>();


        for (int i = 0; i < meciuri; ++i) {
            data[i] = Meci.read(scan);
            Meci m = data[i];
            
            // Pentru fiecare echipa sa avem cheile setate in hash-uri
            gol_dat.put(m.nameAway, gol_dat.getOrDefault(m.nameAway, 0));
            gol_primit.put(m.nameAway, gol_primit.getOrDefault(m.nameAway, 0));
            puncte.put(m.nameAway, puncte.getOrDefault(m.nameAway, 0));
            gol_dat.put(m.nameHome, gol_dat.getOrDefault(m.nameHome, 0));
            gol_primit.put(m.nameHome, gol_primit.getOrDefault(m.nameHome, 0));
            puncte.put(m.nameHome, puncte.getOrDefault(m.nameHome, 0));

            gol_dat.put(m.nameHome, gol_dat.get(m.nameHome) + m.scorHome);
            gol_dat.put(m.nameAway, gol_dat.get(m.nameAway) + m.scorAway);
            
            gol_primit.put(m.nameHome, gol_primit.get(m.nameHome) + m.scorAway);
            gol_primit.put(m.nameAway, gol_primit.get(m.nameAway) + m.scorHome);

            if (m.scorHome > m.scorAway)
                puncte.put(m.nameHome, puncte.get(m.nameHome) + 3);
                
            if (m.scorHome < m.scorAway)
                puncte.put(m.nameAway, puncte.get(m.nameAway) + 3);
        
            if (m.scorHome == m.scorAway) {
                puncte.put(m.nameHome, puncte.get(m.nameHome) + 1);
                puncte.put(m.nameAway, puncte.get(m.nameAway) + 1);
            }
        }

        scan.close();

        int ind = 0;
        for (String nume : gol_dat.keySet()) {
            nume_echipe[ind] = nume;
            ++ind;
        }

        Arrays.sort(nume_echipe,
            (String a, String b) -> puncte.get(b) - puncte.get(a));
        
        for (String nume : nume_echipe) {
            int pts = puncte.get(nume);
            int dat = gol_dat.get(nume);
            int primit = gol_primit.get(nume);
            System.out.println(nume + " " + pts + " " + dat + " " + primit);
        }
    }
}
