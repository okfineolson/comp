package final.question3

import org.apache.spark.SparkContext 
import org.apache.spark.SparkContext._ 
import org.apache.spark.SparkConf 
import org.apache.spark.graphx._
import org.apache.spark.api.java.JavaRDD;
import Array._
object partA {


	def main(args: Array[String]){

		val sc = new SparkContext(conf)
		val docs = sc. parallelize(List((5, "hello world scala"), (2, "this scala program"), (3, "scala creates a pair RDD"), (4, "in spark")))
		// fill your code here, and store the result in a pair RDD InvList
		val InvList = docs.flatMap(Lists=>{
						for(x<-Lists._2.split(" ")) yield (x.toString,Lists._1)
						}
					).sortBy(x=>x._2).groupByKey.map(x=>(x._1,x._2.toSet)).sortBy(x=>x._1)
		
		
		InvList.saveAsTextFile("hdfs://…")
		sc.stop()
	}

}