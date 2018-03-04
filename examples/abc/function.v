module oracle(a, b, c, f);
   input [3:0] a, b, c;
   output      f;

   wire [3:0]  w1, w2;

   assign w1 = a + b;
   assign w2 = b * c;
   assign f  = w1 < w2;
endmodule // oracle
