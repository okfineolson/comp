package comp9313.proj2

import org.apache.spark.SparkContext 
import org.apache.spark.SparkContext._ 
import org.apache.spark.SparkConf 

object Problem1 {


def main(args: Array[String]){
val inputFile = args(0)
val outputFolder = args(1)
//val logFile = "file:///home/comp9313/tiny.txt" //testing file
val conf = new SparkConf().setAppName("Problem1")
val sc = new SparkContext(conf)
val logData = sc.textFile(inputFile)

val words = logData.map(line => line.toLowerCase().split("[\\s*$&#/\"'\\,.:;?!\\[\\](){}<>~\\-_]+")
.filter(x => x.length() > 0 && x.charAt(0) >= 'a' && x.charAt(0) <= 'z')
)
        .flatMap(word=>{
            for (x<-0 to word.length-1;y<-1+x to word.length-1)	
                yield ((word(x),word(y)),1)
        })		
//words.foreach(println)
/*
val output2 = for (x<-0 to words.collect().length-1;y<-1+x to words.collect().length-1)	
                    yield (words.collect().length-1-x,words.collect()(x)+" "+words.collect()(y))

val wordscount = sc.parallelize(output2).countByKey()//reduceByKey(_+_)
*/
val twoset = words.reduceByKey(_+_)
val count = words.map(x=>(x._1._1,x._2)).countByKey()

/*
val a = count.filter{ case (key, value) => key == "a" }
                .map { case (key, value) => value }
println(a.sum)
*/
val answer = twoset.map(x=>(x._1,x._2.toDouble/count
                .filter { case (key, value) => key == x._1._1 }
                .map { case (key, value) => value } 
                .sum.toDouble
                ))
            .sortBy(x=>(x._1._1,-x._2,x._1._2))
            .map(x=>x._1._1+" "+x._1._2+" "+x._2)

/*
for (i <- 0 to words.collect().length-1) {
    for (j <- i+1 to words.collect().length-1){
        println( words.collect()(i)+" "+words.collect()(j),1)
    }
}*/

//answer.foreach(println)
answer.saveAsTextFile(outputFolder)

sc.stop()
}
}