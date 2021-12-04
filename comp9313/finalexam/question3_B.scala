package final.question3

import org.apache.spark.SparkContext 
import org.apache.spark.SparkContext._ 
import org.apache.spark.SparkConf 
import org.apache.spark.graphx._
import Array._
object partB {


def main(args: Array[String]){

    val sc = new SparkContext(conf)
    val edges = sc.textFile(inputFile)
    val pairs = sc. parallelize(List((1, 2, 5), (1, 4, 6), (3, 4, 2), (3, 6, 8)))
    val sourceNode = s.toInt
    val edgeList = pairs.map( x => Edge(x(1).toLong, x(2).toLong, 
    x(3).toDouble))
    val graph = Graph.fromEdges[Int, Double](edgelist, 0)

    val sourceId = s//Input s
    val initialGraph: Graph[Boolean, Double]  = graph.mapVertices((id, _) => id == sourceId)
    val numRNodes = initialGraph.pregel(false)(
        (id, canReach, newCanReach) => canReach || newCanReach, // Vertex Program
        triplet => {  // Send Message
        if (triplet.srcAttr && !triplet.dstAttr) {
            Iterator((triplet.dstId, true))
        } else {
            Iterator.empty
        }
        },
        (a, b) => a || b // Merge Message
    )


    println(numRNodes.count())//print the count of nodes
    sc.stop()

}