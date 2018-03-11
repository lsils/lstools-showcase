package abc

import com.sun.jna._
import com.sun.jna.ptr._
import play.api.libs.json._

class ABC {
  trait Lib extends Library {
    def abc2_create(): IntByReference
    def abc2_delete(cli: IntByReference)
    def abc2_command(cli: IntByReference, command: String, log: Array[Byte], size: Int): Int
  }

  System.setProperty("jna.library.path", "../../../build/examples/abc")
  val lib = Native.loadLibrary("abc2_c", classOf[Lib]).asInstanceOf[Lib]
  val cli = lib.abc2_create()

  def run(command: String): JsValue = {
    val buffer = new Array[Byte](4096)
    val length = lib.abc2_command(cli, command, buffer, 4096)

    if (length > 0) {
      var str = Native.toString(buffer).asInstanceOf[String]
      return Json.parse(str)
    }

    return null
  }

  def close(): Unit = {
    lib.abc2_delete(cli)
  }
}

object Program {
  def main(args: Array[String]) {
    val abc = new ABC
    abc.run("read_verilog ../function.v")
    abc.run("ps -w")
    abc.run("convert --wlc_to_aig")
    val nodes_before = abc.run("ps -a")("nodes").as[Int]
    abc.run("syn3")
    val nodes_after = abc.run("ps -a")("nodes").as[Int]
    abc.close

    if (nodes_after < nodes_before) {
      println("Improvement :)")
    } else {
      println("No improvement :(")
    }
  }
}
