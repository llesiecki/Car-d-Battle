<?php

		function getwinners($player_num,$parameteres,$category)
		{
			$winners=array_fill(0,$player_num, true);
			for($i=0;$i<$player_num;$i++)
			{
				for($j=0;$j<$player_num;$j++)
				{
					if($i==$j){continue;}
					$ivalue=$parameteres[$i][$category];
					$jvalue=$parameteres[$j][$category];
					if($category=='acceleration')
					{
						if($ivalue>$jvalue)
						{
							$winners[$i]=false;
						}
					}
					else
					{
						if($ivalue<$jvalue)
						{
							$winners[$i]=false;
						}
					}
				}
			}
			return $winners;
		}
		$cards =range(1,24);
		shuffle($cards);
		$cardsstr='';
		$player_num=4;
		$player_stack=array();
		//category tymczasowe
		$category='topspeed';
		
		//card id from string to char central_stack
		for($i=0; $i<24; $i++)
		{
			if(!isset($player_stack[$i%$player_num]))
				$player_stack[$i%$player_num]='';

			$cardsstr=$cardsstr.chr($cards[$i]+64);
			$player_stack[$i%$player_num]=$player_stack[$i%$player_num].chr($cards[$i]+64);
		}
		echo $cardsstr."<br/>";
		echo implode(" ",$player_stack);
		
		$cardidplay1=11;
		$cardidplay2=21;
		$cardidplay3=2;
		$cardidplay4=23;

		require_once "connect.php";
		mysqli_report(MYSQLI_REPORT_STRICT);
		
		try
		{
			$connection= new mysqli($host, $db_user, $db_password, $db_name);
		
			if ($connection->connect_errno!=0)
				{
						throw new Exception(mysqli_connect_errno());
				}
			else
				{
					//sending card keys
					//if($connection->query("INSERT INTO game values ('','$player_stack[0]','','$player_stack[1]','','$player_stack[2]','','$player_stack[3]','','','')"))
					{}
				}	
			
			$compcards=array();
				
				
				$sql = "UPDATE game SET player_1_hand = CONCAT(player_1_hand, (SELECT right(player_1_stack,1) from game where idgame='21')) where idgame='21'";
				$result = $connection->query($sql);
				$sql = "UPDATE game SET player_1_stack = (SELECT REPLACE (player_1_stack, right(player_1_stack,1), ' ') from game where idgame='21') where idgame='21'";
				$result = $connection->query($sql);
				$sql = "SELECT right(player_1_hand,1) ,right(player_2_hand,1),right(player_3_hand,1),right(player_4_hand,1) FROM game where idgame='21'";
				$result = $connection->query($sql);
				$player_hand_top = $result->fetch_assoc();
				echo "<br/>".implode(" ",$player_hand_top)."<br/>";
				
				print_r ($player_hand_top);
				
			$parameteres=array();
			for($i=1; $i<5;$i++)
			{
				echo "<br/>";
				$idcard=ord($player_hand_top['right(player_'.$i.'_hand,1)'])-64;
				$sql="select * from deck where idcard='$idcard'";
				$parameteres[$i-1] = $connection->query($sql)->fetch_assoc();
				print_r($parameteres[$i-1][$category]);
			}
			
			$winners=getwinners($player_num,$parameteres,$category);
			
				print_r ($winners);
			
			//tiebreak
			$winners_num=array_sum($winners);
			
			/*while($winners_num!=1)
			{
				$empty=array_fill(0,$player_num, false);//0-pozycja startowa tablicy, $player_num-do którego wyrazu, false-wartość
				
				for($i=0;$i<2;$i++)
				{
					for($j=0;$j<$player_num;$i++)
					{
						if($winners[$j]==true)
						{
							
						}
					}
				}
				
				
				
			}*/ 
								
						
				
		}
		
		catch(Exception $e)
		{
			echo '<span style="color:red;">Server ERROR</span>';
			echo '<br/>DEV INFO: '.$e;
		}
		
			$connection->close();
		//print_r($cards);
		//print_r($owners);
		//print_r(implode($cards));
		
		
		

?>