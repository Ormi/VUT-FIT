      <?    $query="DELETE FROM liecba WHERE cislo_liecby='".$_GET["c"]."'";
          $result = @mysql_query($query,$db);
          ?>
          Zaznam o liecbe bol uspesne odstraneny.
          <?