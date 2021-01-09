<?php
 
DEFINE ('DEBUG', false);
 
$poolOfDigits_box_1 = array(1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5);
 
$recordsCounter = 0;
$maxRecordsToTest = 0;
 
$totalGoodPermutations = 0;
$totalBadPermutations = 0;
 
function printPoolOfDigits($boxNumber, $poolOfDigits) {
    
    if (!DEBUG) return;
        
    echo "pool of digits for box {$boxNumber}: ";
    
    foreach ($poolOfDigits as $digit) {
        echo "{$digit}|";
    }
    
    echo PHP_EOL;
}
 
function formatBoxes($box_1_digits, $box_2_digits, $box_3_digits, $box_4_digits, $box_5_digits) {
 
    $outputString = "{$box_1_digits[0]}{$box_1_digits[1]}{$box_1_digits[2]}{$box_1_digits[3]}{$box_1_digits[4]} ".  
                    "{$box_2_digits[0]}{$box_2_digits[1]}{$box_2_digits[2]}{$box_2_digits[3]}{$box_2_digits[4]} ".
                    "{$box_3_digits[0]}{$box_3_digits[1]}{$box_3_digits[2]}{$box_3_digits[3]}{$box_3_digits[4]} ".
                    "{$box_4_digits[0]}{$box_4_digits[1]}{$box_4_digits[2]}{$box_4_digits[3]}{$box_4_digits[4]} ".
                    "{$box_5_digits[0]}{$box_5_digits[1]}{$box_5_digits[2]}{$box_5_digits[3]}{$box_5_digits[4]}";
               
    return $outputString;
}
 
function printUniquePermutations($boxNumber, $permutations) {
    
    if (!DEBUG) return;
    
    echo "unique permutations for box {$boxNumber}". PHP_EOL;
    
    $i=0;
    foreach ($permutations as $perm) {
        $i++;
        echo "[{$i}] ". $perm[0]. $perm[1]. $perm[2]. $perm[3]. $perm[4]. PHP_EOL;
    }
    
}
 
function removeDigitsFromPool($pool, $digitsToRemove) {
 
    $newPool = array();
    
    $poolSize = count($pool);
 
    for ($i=0; $i<$poolSize; $i++) {        
        if ($pool[$i] == $digitsToRemove[0]) {
            $pool[$i] = 0;
            break;
        }         
    }
    
    for ($i=0; $i<$poolSize; $i++) {
        if ($pool[$i] == $digitsToRemove[1]) {
            $pool[$i] = 0;
            break;
        }
    }
    
    for ($i=0; $i<$poolSize; $i++) {
        if ($pool[$i] == $digitsToRemove[2]) {
            $pool[$i] = 0;
            break;
        }
    }
    
    for ($i=0; $i<$poolSize; $i++) {
        if ($pool[$i] == $digitsToRemove[3]) {
            $pool[$i] = 0;
            break;
        }
    }
    
    for ($i=0; $i<$poolSize; $i++) {
        if ($pool[$i] == $digitsToRemove[4]) {
            $pool[$i] = 0;
            break;
        }
    }
    
    for ($i=0; $i<$poolSize; $i++) {
        if ($pool[$i] != 0) {
            $newPool[] = $pool[$i];
        }
    }
    
    return $newPool;
}
 
function getUniquePermutationsFromPool(&$pool, $previous_box_digits) {
   
    $poolSize = count($pool);
    
    $uniquePermutations = array();
    $uniquePermutationsIntValues = array();
    
    $previous_box_int_value = 10000 * $previous_box_digits[0] + 
                              1000 * $previous_box_digits[1] + 
                              100 * $previous_box_digits[2] + 
                              10 * $previous_box_digits[3] + 
                              $previous_box_digits[4];
    $permIx = 0;
    
    for ($g=0; $g<$poolSize-4; $g++) {
        
        for ($h=$g+1; $h<$poolSize-3; $h++) {
        
            for ($i=$h+1; $i<$poolSize-2; $i++) {
            
                for ($j=$i+1; $j<$poolSize-1; $j++) {
                    
                    for ($k=$j+1; $k<$poolSize; $k++) {
                        
                        $intValue = 10000 * $pool[$g] + 1000 * $pool[$h] + 100 * $pool[$i] + 10 * $pool[$j] + $pool[$k];
                        
                        if ($intValue < $previous_box_int_value) continue;
                        
                        if (!in_array($intValue,$uniquePermutationsIntValues)) {
                            
                            $uniquePermutations[$permIx][0] = $pool[$g];
                            $uniquePermutations[$permIx][1] = $pool[$h];
                            $uniquePermutations[$permIx][2] = $pool[$i];
                            $uniquePermutations[$permIx][3] = $pool[$j];
                            $uniquePermutations[$permIx][4] = $pool[$k];
                            
                            $uniquePermutationsIntValues[$permIx] = $intValue;
        
                            $permIx++;
                        }
                    }
                }
            }
        }
    }
    
    return $uniquePermutations;
}
 
function calculateNumOfCombinationForBox($box_digits) {
    
    $numberOfIdenticalDigits = array();
    
    $numberOfIdenticalDigits[1] = countDigits(1, $box_digits);
    $numberOfIdenticalDigits[2] = countDigits(2, $box_digits);
    $numberOfIdenticalDigits[3] = countDigits(3, $box_digits);
    $numberOfIdenticalDigits[4] = countDigits(4, $box_digits);
    $numberOfIdenticalDigits[5] = countDigits(5, $box_digits);
    
    if ($numberOfIdenticalDigits[1] == 5 ||
        $numberOfIdenticalDigits[2] == 5 ||
        $numberOfIdenticalDigits[3] == 5 ||
        $numberOfIdenticalDigits[4] == 5 ||
        $numberOfIdenticalDigits[5] == 5 ) {
            
            // there are 5 identical digits (i.e., xxxxx)
            return 1;
    }
                
    if ($numberOfIdenticalDigits[1] == 4 ||
        $numberOfIdenticalDigits[2] == 4 ||
        $numberOfIdenticalDigits[3] == 4 ||
        $numberOfIdenticalDigits[4] == 4 ||
        $numberOfIdenticalDigits[5] == 4 ) {
            
            // there are 4 identical digits (e.g., xxxxy)
            return 5;
    }
        
    if ($numberOfIdenticalDigits[1] == 3 || 
        $numberOfIdenticalDigits[2] == 3 || 
        $numberOfIdenticalDigits[3] == 3 || 
        $numberOfIdenticalDigits[4] == 3 ||
        $numberOfIdenticalDigits[5] == 3 ) {
            
        if ($numberOfIdenticalDigits[1] == 2 ||
            $numberOfIdenticalDigits[2] == 2 ||
            $numberOfIdenticalDigits[3] == 2 ||
            $numberOfIdenticalDigits[4] == 2 ||
            $numberOfIdenticalDigits[5] == 2 ) {
                // there are 3 identical digits and one pair (i.e., xxxyy, xxyxy, xxyyx, xyxyx, ...)
                return 10; // (5!/(3!*2!)) 
            } else {
                // there are 3 identical digits and 2 different digits (i.e., xxxyz, xxyzx, ...)
                return 20; // (5!/(3!*2!)) * 2!
            }
    } 
    
    $numOfPairs = 0;
    for ($i=1; $i<=5; $i++) {
        if ($numberOfIdenticalDigits[$i] == 2) $numOfPairs++;
    }
    
    if ($numOfPairs == 0) return 120; // 5! as there are 5 non-identical digits (i.e., vwxyz)
    if ($numOfPairs == 1) return 60; // there is one pair and 3 non-identicals (i.e., xxwyz, xwyxz, xwyzx, ....)
    if ($numOfPairs == 2) return 30; // (5!/(2!*2!)) as there are 2 sets of 2 identical digits (i.e., xxyyz, xyxyz, yxyxz, ...) 
}
 
 
function countDigits($digit, $box_digits) {
    
    $matchingDigits = 0;
    
    for($i=0; $i<=4; $i++) {
        if ($box_digits[$i] == $digit) $matchingDigits++;        
    }
    
    return $matchingDigits;
}
 
function areBoxesGood($box_1_digits, $box_2_digits, $box_3_digits, $box_4_digits, $box_5_digits) {
    
    $digitsOccurance = array();
    
    $digitsOccurance[1][1] = countDigits(1, $box_1_digits);
    $digitsOccurance[1][2] = countDigits(2, $box_1_digits);
    $digitsOccurance[1][3] = countDigits(3, $box_1_digits);
    $digitsOccurance[1][4] = countDigits(4, $box_1_digits);
    $digitsOccurance[1][5] = countDigits(5, $box_1_digits);
    
    $digitsOccurance[2][1] = countDigits(1, $box_2_digits);
    $digitsOccurance[2][2] = countDigits(2, $box_2_digits);
    $digitsOccurance[2][3] = countDigits(3, $box_2_digits);
    $digitsOccurance[2][4] = countDigits(4, $box_2_digits);
    $digitsOccurance[2][5] = countDigits(5, $box_2_digits);
    
    $digitsOccurance[3][1] = countDigits(1, $box_3_digits);
    $digitsOccurance[3][2] = countDigits(2, $box_3_digits);
    $digitsOccurance[3][3] = countDigits(3, $box_3_digits);
    $digitsOccurance[3][4] = countDigits(4, $box_3_digits);
    $digitsOccurance[3][5] = countDigits(5, $box_3_digits);
    
    $digitsOccurance[4][1] = countDigits(1, $box_4_digits);
    $digitsOccurance[4][2] = countDigits(2, $box_4_digits);
    $digitsOccurance[4][3] = countDigits(3, $box_4_digits);
    $digitsOccurance[4][4] = countDigits(4, $box_4_digits);
    $digitsOccurance[4][5] = countDigits(5, $box_4_digits);
    
    $digitsOccurance[5][1] = countDigits(1, $box_5_digits);
    $digitsOccurance[5][2] = countDigits(2, $box_5_digits);
    $digitsOccurance[5][3] = countDigits(3, $box_5_digits);
    $digitsOccurance[5][4] = countDigits(4, $box_5_digits);
    $digitsOccurance[5][5] = countDigits(5, $box_5_digits);
    
    $boxesStatuses = array(true, false, false, false, false, false); // assume BAD on all 5 boxes
    
    for ($i=1; $i<=5; $i++) {
        
        $boxIsOK = false;
        
        for ($j=1; $j<=5; $j++) {
            
            if ($digitsOccurance[$i][$j] < 2) continue;
            
            $boxIsOK = true;
            
            for ($k=1; $k<=5; $k++) {
                
                if ($k==$i) continue;
                
                if ($digitsOccurance[$k][$j] >= $digitsOccurance[$i][$j]) {
                    
                    $boxIsOK = false;
                    break;
                }                                
            }
            
            if ($boxIsOK) break;
        }
        
        $boxesStatuses[$i] = $boxIsOK;
    }
    
    return ($boxesStatuses[1] && $boxesStatuses[2] && $boxesStatuses[3] && $boxesStatuses[4] && $boxesStatuses[5]);
}
 
function calcNumberOfPermutationsForAllFiveBoxes($box_1_digits, $box_2_digits, $box_3_digits, $box_4_digits, $box_5_digits) {
    
    $boxes_int_value = array();
    $boxes_int_value[0] = 10000 * $box_1_digits[0] + 1000 * $box_1_digits[1] + 100 * $box_1_digits[2] + 10 * $box_1_digits[3] + $box_1_digits[4];
    $boxes_int_value[1] = 10000 * $box_2_digits[0] + 1000 * $box_2_digits[1] + 100 * $box_2_digits[2] + 10 * $box_2_digits[3] + $box_2_digits[4];
    $boxes_int_value[2] = 10000 * $box_3_digits[0] + 1000 * $box_3_digits[1] + 100 * $box_3_digits[2] + 10 * $box_3_digits[3] + $box_3_digits[4];
    $boxes_int_value[3] = 10000 * $box_4_digits[0] + 1000 * $box_4_digits[1] + 100 * $box_4_digits[2] + 10 * $box_4_digits[3] + $box_4_digits[4];
    $boxes_int_value[4] = 10000 * $box_5_digits[0] + 1000 * $box_5_digits[1] + 100 * $box_5_digits[2] + 10 * $box_5_digits[3] + $box_5_digits[4];
    
    $matches = array(1,1,1,1,1);
    
    for ($i=0; $i<4; $i++) {
        
        if ($boxes_int_value[$i] == 0) continue;
        
        for ($j=$i+1; $j<5; $j++) {
            
            if ($boxes_int_value[$j] == 0) continue;
            
            if ($boxes_int_value[$i] == $boxes_int_value[$j]) {
                $matches[$i]++;
                $boxes_int_value[$j] = 0;
            }
        }        
    }
 
    $totalCombinations = 120; // 5!
    
    foreach ($matches as $numBoxesIdentical) {
        switch ($numBoxesIdentical) {
            case 2:
                $totalCombinations = $totalCombinations / 2; 
                break;
            case 3:
                $totalCombinations = $totalCombinations / 6; // 3! 
                break;
            case 4:
                $totalCombinations = 5; // (5!/4!)
                break;
            case 5:
                $totalCombinations = 1; // 5!/5!
                break;
            default:
                break;
        }
    }
    
    return $totalCombinations;
    
}
 
$file = fopen ("e:/temp-israel/candy.txt",'w');
 
$box_1_permutations = getUniquePermutationsFromPool($poolOfDigits_box_1, array(0,0,0,0,0));
 
printPoolOfDigits(1, $poolOfDigits_box_1);
 
printUniquePermutations(1, $box_1_permutations);
 
foreach ($box_1_permutations as $box_1_digits) {
    
    if ($box_1_digits[0] == 2) break;
    
    $poolOfDigits_box_2 = removeDigitsFromPool($poolOfDigits_box_1, $box_1_digits);
    
    printPoolOfDigits(2, $poolOfDigits_box_2);
    
    $box_2_permutations = getUniquePermutationsFromPool($poolOfDigits_box_2, $box_1_digits);
    
    printUniquePermutations(2, $box_2_permutations);
    
    foreach ($box_2_permutations as $box_2_digits) {
                
        $poolOfDigits_box_3 = removeDigitsFromPool($poolOfDigits_box_2, $box_2_digits);
        
        printPoolOfDigits(3, $poolOfDigits_box_3);
        
        $box_3_permutations = getUniquePermutationsFromPool($poolOfDigits_box_3, $box_2_digits);
        
        printUniquePermutations(3, $box_3_permutations);
        
        foreach ($box_3_permutations as $box_3_digits) {
            
            $poolOfDigits_box_4 = removeDigitsFromPool($poolOfDigits_box_3, $box_3_digits);
            
            printPoolOfDigits(4, $poolOfDigits_box_4);
            
            $box_4_permutations = getUniquePermutationsFromPool($poolOfDigits_box_4, $box_3_digits);
            
            printUniquePermutations(4, $box_4_permutations);
            
            foreach ($box_4_permutations as $box_4_digits) {
                
                $poolOfDigits_box_5 = removeDigitsFromPool($poolOfDigits_box_4, $box_4_digits);
                
                printPoolOfDigits(5, $poolOfDigits_box_5);
                
                $box_5_permutations = getUniquePermutationsFromPool($poolOfDigits_box_5, $box_4_digits); 
 
                /*
                $box_5_permutations = array(array($poolOfDigits_box_5[0], 
                                                  $poolOfDigits_box_5[1], 
                                                  $poolOfDigits_box_5[2], 
                                                  $poolOfDigits_box_5[3], 
                                                  $poolOfDigits_box_5[4]));                
                */
                
                printUniquePermutations(5, $box_5_permutations);
                
                foreach ($box_5_permutations as $box_5_digits) {
                    
                    $recordsCounter++;
                    
                    $box_1_combinations = calculateNumOfCombinationForBox($box_1_digits);
                    $box_2_combinations = calculateNumOfCombinationForBox($box_2_digits);
                    $box_3_combinations = calculateNumOfCombinationForBox($box_3_digits);
                    $box_4_combinations = calculateNumOfCombinationForBox($box_4_digits);
                    $box_5_combinations = calculateNumOfCombinationForBox($box_5_digits);
                    
                    $totalCombinations = $box_1_combinations *
                                         $box_2_combinations *
                                         $box_3_combinations *
                                         $box_4_combinations *
                                         $box_5_combinations; 
                    
                    // how many boxes are equal?
                    $permutationsBetweenTheBoxes = calcNumberOfPermutationsForAllFiveBoxes($box_1_digits, $box_2_digits, $box_3_digits, $box_4_digits, $box_5_digits);
                                        
                    $totalCombinations *= $permutationsBetweenTheBoxes;
                    
                    $output = formatBoxes($box_1_digits, $box_2_digits, $box_3_digits, $box_4_digits, $box_5_digits);
                    $desc = "  -  {$box_1_combinations} x {$box_2_combinations} x {$box_3_combinations} x {$box_4_combinations} x {$box_5_combinations} x {$permutationsBetweenTheBoxes} = {$totalCombinations}";
                    
                    if (areBoxesGood($box_1_digits, $box_2_digits, $box_3_digits, $box_4_digits, $box_5_digits)) {
                        
                        $totalGoodPermutations += $totalCombinations;
                        
                        $status = " Ok  ";
                        
                    } else {
                        
                        $totalBadPermutations += $totalCombinations;
                        
                        $status = " Bad ";
                        
                    }                    
 
                    $record = "[{$recordsCounter}] {$status}  {$output} {$desc}". PHP_EOL;
                    
                    fwrite($file, $record);
                    
                    echo $record;
                    
                    if ($recordsCounter == $maxRecordsToTest) break;
                }
                if ($recordsCounter == $maxRecordsToTest) break;
            }
            if ($recordsCounter == $maxRecordsToTest) break;
        }
        if ($recordsCounter == $maxRecordsToTest) break;
    }
    if ($recordsCounter == $maxRecordsToTest) break;
}
 
fclose($file);
 
echo PHP_EOL. PHP_EOL;
echo "Number of records created: {$recordsCounter}". PHP_EOL;
echo "Number of GOOD permutations: {$totalGoodPermutations}". PHP_EOL; 
echo "Chances are: {$totalGoodPermutations} / ". ($totalGoodPermutations + $totalBadPermutations). " = ". ($totalGoodPermutations / ($totalGoodPermutations + $totalBadPermutations)). PHP_EOL;