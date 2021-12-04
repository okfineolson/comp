package comp9313.proj3

import org.apache.spark.SparkContext 
import org.apache.spark.SparkContext._ 
import org.apache.spark.SparkConf 
import org.apache.spark.graphx._
import org.apache.spark.api.java.JavaRDD;
import Array._
object SetSimJoin {


	def main(args: Array[String]){
		val inputFile = args(0)
		val outputFolder = args(1)
		val given_τ = args(2).toDouble
		val conf = new SparkConf().setAppName("Problem1")
		val sc = new SparkContext(conf)
		val input = sc.textFile(inputFile)
			
		val dataset = input.map(line=>line.split(" "))
		
		val record = dataset.flatMap(line=>{
		//take first |x|-ceil(|x|*t)+1
			for(x <- 0 to (line.length - Math.ceil((line.length-1)*given_τ) -1).toInt) 
				yield (line.toList.drop(1).map(x=>x.toInt).sortWith(_ < _)(x).toInt,(line(0).toInt,line.toList.drop(1).map(x=>x.toInt).sortWith(_ < _).toSet))
		}).groupByKey.map(x=>x._2.toSet)
		
		//record.foreach(println)
		val jaccard_set = record.flatMap(xset=>{
		for(x<-xset;y<-xset if( (x._1 != y._1) 
					&& (x._1.toDouble < y._1.toDouble) 
					&& (
						  ((x._2.size > y._2.size) &&(x._2.size*given_τ <= y._2.size))
					    	||((x._2.size <= y._2.size) &&(y._2.size*given_τ <= x._2.size))
				    	   )
			    	   	&& (Similarity(x._2,y._2) >= given_τ)
			    	   	
				       )
		    )
			yield (x,y)
		})
		//record.foreach(println)
		val jaccard_output = jaccard_set
						.map(x=>((x._1._1,x._2._1),Similarity(x._1._2,x._2._2)))
						.distinct
						.sortBy(x=>(x._1._1,x._1._2))
						.map(x=>x._1+" "+x._2)
		//jaccard_output.foreach(println)
		jaccard_output.saveAsTextFile(outputFolder)
		//output.map(x=>x._2.filter { case x => x.startsWith(x._1.toString) }).foreach(println)
		//println(res.vertices.collect().mkString("\n"))
		sc.stop()
	}
	def Similarity (a:Set[Int], b:Set[Int]) : Double = {
		var out1 = (a&b).size.toDouble
		var out2 = (a++b).size.toDouble
		return out1/out2
	}
}