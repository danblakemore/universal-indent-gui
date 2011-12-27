#!/usr/bin/env ruby 
#odd assignments
BEGIN {
	puts "a block i have never seen used"
}
entry = Post.update(params["id"],{:title => params["title"],:post => params['post'],:context => params["context"],:creator => session[:creator]})
definition = "moo"
puts moo
moo = case 3
when 2
	"unless proceeding to 3"
when 3
	"right"
when 4
	"one to many"
when 5
	"three sir"
end

puts moo
def pointless_call
	if false
		"Sdf"
	elsif true
		"df"
	end
end
puts pointless_call

if true
	puts "moo"
end
i = 5
def title
	doc = load_page
	title = doc.search("h1").first.inner_html
	clean_html_tags(title)
	clean_9_0(title)
	title
end
if i
	if true
		puts "moo"
	elsif i < 3 * 23
		"sdf"
	else
		"df"
	end
end
class Tested
	def sadf
		"asdf"
	end
end

module Moo
	def t434t
		"352"
	end#comments at the end
end #comments again debug_if

=begin
block comments
    should have no formatting done
ever
=end

#java formatter test parts

ping(argument) {|block|

}

if (moo)
	cow;
else
	dog;
end

x = 5
x = 5
x = 5
x  =  5
IN_OUTS_RX = /^(def|class|module|begin|case|if|unless|loop|while|until|for)/

#end java formatter test parts

here_doc = <<-EOX
This should not
loose its formatting
EOX
dsfffffffff=[2, 3, 4, 5]
print <<-STRING1, <<-STRING2
Concat
STRING1
enate
STRING2

unless false
	"4"
else
	"5"
end
x = 2
while x > 0
	x -= 1
	if x == 1
		"p"
	else
		"3"
	end
end
x = 2
until x < 0
	x -= 1
end
a = 3
a *= 2 while a < 100
a -= 10 until a < 100
print "Hello\n" while false
print "Goodbye\n" while false
3.times do
	print "Ho! "
end
0.upto(9) do | x|
	print x, " "
end
0.step(12, 3) {|x | print x, " " }
x = 0
loop {
	if x == 5
		break
	end
	x += 1
}
(1..4).each {|x|
	puts x
}
(1..4).each do | x|
	puts x
end
for i in (1..4)
	puts i
end
i = 0
loop do
	i += 1
	next if i < 3
	print i
	break if i > 4
end
string = "x+1"
begin
	eval string
rescue SyntaxError, NameError => boom
	print "String doesn't compile: " + boom
rescue StandardError => bang
	print "Error running script: " + bang
ensure
	print "thank you pick axe"
end
a = "Fats ' ' \\\" do Waller"
a =~ /\/a/
if true then print "a" end
x = 3
unless  true then print "a" end
x = 3
begin raise "cow"
rescue Exception => e
end
x = 3
puts i += 1 while i < 3 # ruby
x = 3
klass = Fixnum
#its like a do while loop
begin
	print klass
	klass = klass.superclass
	print " < " if klass
end while klass
puts
p Fixnum.ancestors
boom = %q / this is a spinal tap/
boom = %q - string-
boom =%q(a (nested) string)
x = "done with string"

puts "In parent,term = #{ENV['TERM']}"
cow = if true
	"moot"
else
	"woot"
end

fork do
	puts "Start of child 1,term=#{ENV['TERM']}"
	ENV['TERM'] = "ansi"
	fork do
		puts "Start of child 2, term=#{ENV['TERM']}"
		begin
			if moo < 3
				p "asdf4"
			elsif 9 * 0
				p "asde"
			else
				puts cow
			end
		end	while x > 3
	end
	Process.wait
	puts "End of child 1, term=#{ENV['TERM']}"
end
Process.wait
puts "Back in parent, term=#{ENV['TERM']}"

OPENOFFICE = true  # do Openoffice - Spreadsheet Tests?
EXCEL      = true  # do Excel Tests?
GOOGLE     = true # do Google - Spreadsheet Tests?

OPENOFFICEWRITE = false # experimental:

END{
	puts "another block i have never seen"
}