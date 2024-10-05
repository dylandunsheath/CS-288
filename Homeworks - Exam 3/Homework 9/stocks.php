<?php
// Refreshes every 3 minutes
// Source: https://www.geeksforgeeks.org/refresh-a-page-using-php/
header("refresh: 180");
require "vendor/autoload.php";
$client = new MongoDB\Client();
$database = $client -> stock_database;
$collection = $database -> most_active_stocks;
$result = $collection->find([]);
?>

<!DOCTYPE html>
<html>
    <head>
        <title>Active Stocks</title>
        <!-- https://mottie.github.io/tablesorter/docs/ -->
        <!-- https://www.sitepoint.com/sorting-tables-tablesorter/ -->
        <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery.tablesorter/2.31.3/js/jquery.tablesorter.min.js"></script>
    </head>
    <body>
        <center><h1>Most Active Stocks</h1></center>
        <center><h2>Created by Dylan Dunsheath (CS-288-006)</h2></center>
        <center><h3>Source: <a href="https://finance.yahoo.com/most-active">Click here</a></h3></center>
        <style>
            table {
                border-collapse: collapse;
                width: 50%;
            }
            .sortable {
                text-decoration: underline;
                color: blue;
            }
            th, td {
                border: 2px solid black;
                padding: 8px;
                text-align: center;
            }
        </style>
        <table id="stockTable" border=2 align="center">
            <thead>
                <tr>
                    <th class="sortable">Index</th>
                    <th class="sortable">Symbol</th>
                    <th class="sortable">Name</th>
                    <th class="sortable">Price (Intraday)</th>
                    <th class="sortable">Change</th>
                    <th class="sortable">Volume</th>
                </tr>
            </thead>
            <tbody>
                <?php
                $index = 1;
                foreach($result as $doc){
                    echo "<tr>";
                    echo "<td>{$index}</td>";
                    foreach($doc as $key=>$value){
                        if($key != "_id") {
                            echo "<td>{$value}</td>";
                        }
                    }
                    echo "</tr>";
                    $index++;
                }
                ?>
            </tbody>
        </table>
        <script>
            $(document).ready(function(){
            $("#stockTable").tablesorter();
        });
        </script>
    </body>
</html>