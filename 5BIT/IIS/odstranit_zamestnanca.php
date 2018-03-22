  <?    $query="DELETE FROM zamestnanec WHERE rodne_cislo='".$_GET["c"]."'";
      $result = @mysql_query($query,$db);
      ?>
      Zaznam o zamestnancovi bol úspešne odstránený.
      <?
