name := """abc2"""

mainClass in Compile := Some("abc.Program")

libraryDependencies += "net.java.dev.jna" % "jna" % "4.0.0"
libraryDependencies += "com.typesafe.play" %% "play-json" % "2.6.7"
