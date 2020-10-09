<?php
		parse_str(getenv("QUERY_STRING"));
    		$conn = mysqli_connect("localhost","user","passwd", "db") or die("fail");

    		$dbconn = mysqli_select_db($conn, $mysql_db);

    		mysqli_query("set names utf 8");

    		$query = "select alert from membership where card_id='".$card_id."'";
    		
    		$result = mysqli_query($conn, $query);
		$row = mysqli_fetch_array($result);
		$input = $row[alert]+1 ;
		$query2 = "update membership set alert=".$input." where card_id='".$card_id."'";
		
		$result2 = mysqli_query($conn, $query2);
		mysqli_close($conn);

		echo("<meta http-equiv='Refresh' content='1; URL=index.html'>");

    	?>
