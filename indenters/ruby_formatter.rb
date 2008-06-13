#!/usr/bin/ruby -w
#
# == Synopsis
#
# Simple Ruby Formatter
#
# Created by: Stephen Becker IV
# Contributions: Andrew Nutter-Upham
# Contact: sbeckeriv@gmail.com
# SVN: http://svn.stephenbeckeriv.com/code/ruby_formatter/
#
# Its been done before RadRails did,
# http://vim.sourceforge.net/tips/tip.php?tip_id = 1368 that guy did it, but I did
# not look for a ruby formatter until i was done.
#
# It is called simple formatting because it is. I have the concept of 3 differnt
# indent actions In, Out and Both. I have mixed the concept of indenting and
# outdenting. Out means you have added white space and in means you remove a layer
# of white space.
#
# Basic logic
# 	Decrease current depth if
# 			((the it is not a one line if unless statment
# 			(need to lookfor more oneline blocks) and it ends with end
# 			or if the } count is larger then {)
# 		or
# 			the first word is in the both list)
#
# 		and
# 			depth is larger then zero
#
# 	Increase current depth if
# 			It is not a one liner
# 		and
# 			(the word is in the out list
# 		or
# 			the word is in the both list
# 		or
# 			it looks like a start block)
# 		and
# 			temp_depth is nil (used for = comment blocks)
#
#
# Sure there are some regx's and a crap load of gsubs, but it still simple. Its
# not like its a pychecker (http://www.metaslash.com/brochure/ipc10.html)
#
# == Usage
#
# ruby [options] filelist
#
# options:
#   -s # will change the indent to a space count of # per level
#        by default we space with 1 tab per level
#   -b # create a backup file
#
# examples:
# ruby simple_formatter.rb -s 3 -b /moo/cluck/cow.rb
# runs with the indent of 3 spaces,creates a backup file, and formats moo/cluck/cow.rb
#
#
# Tested with random files off of koders.com
#
#
::DEBUG_ME = false
require 'getoptlong'
require "fileutils"
require "pp"
$escape_strings = {:regex=>"EsCaPedReGex",:string=>"EsCaPeDStRiNg"}
begin
	require 'rdoc/usage'
rescue Exception => e
	#eat the no load of rdocs?
end
opts = GetoptLong.new(
[ '--help', '-h', GetoptLong::NO_ARGUMENT ],
[ '--spaces', '-s', GetoptLong::OPTIONAL_ARGUMENT ],
[ '--debug', '-d', GetoptLong::NO_ARGUMENT ],
[ '--backup', '-b', GetoptLong::NO_ARGUMENT ]
)
space_count = nil
backup = false
files = []
opts.each do | opt, arg|
	case opt
	when '--help'
		begin
			RDoc::usage
		rescue Exception =>e
			puts "If you want to use rdocs you need to install it"
			exit(-1)
		end
	when '--spaces'
		space_count = arg.to_i
	when '--backup'
		backup = true
	when '--debug'
		::DEBUG_ME = true
	end
end
require "profile" if ::DEBUG_ME
if ARGV.length < 1
	puts "Missing filelist argument (try --help)"
	exit 0
end
array_loc = ARGV
#find if the string is a start block
#return true if it is
#rules
# does not include end at the end
# and ( { out number the } or it includes do
DO_RX = /\sdo\s*$/
def start_block?(string)
	return true if  string.gsub(/\|.*\|/, "").match(DO_RX) || (string.scan(/\{/).size > string.scan(/\}/).size)
	false
end
#is this an end block?
#rules
#its not a one liner
#and it ends with end
#or } out number {
CHECK_ENDS_RX = /end$|end\s+while/
def check_ends?(string)
	#check for one liners end and }
	#string = just_the_string_please(string)
	return true if (string.scan(/\{/).size < string.scan(/\}/).size) || string.match(CHECK_ENDS_RX)
	false
end
IN_OUTS_RX = /^(def|class|module|begin|case|if|unless|loop|while|until|for)/
#look at first work does it start with one of the out works
def in_outs?(string)
	string.sub!(/\(.*\)/, "")
	return true if string.lstrip.match(IN_OUTS_RX) && string.strip.size == $1.strip.size
	false
end
IN_BOTH_RX = /^(elsif|else|when|rescue|ensure)/
#look at first work does it start with one of the both words?
def in_both?(string)
	return true if string.lstrip.match(IN_BOTH_RX) && string.strip.size == $1.strip.size
	false
end
#extra formatting for the line
#we wrap = with spaces
#JUST_STRING_PLEASE_RX = /^#.*|\/.*\/|"([^"])*"|'([^'])*'/
LINE_CLEAN_UP_RX = /[a-zA-Z\]\'\"{\d]+=[a-zA-Z\[\'\"{\d]+/
def line_clean_up(x)
	#this formatts strings and regexs remove and add in replacement works

	x.gsub!(/\\\//,$escape_strings[:regex])
	strings = x.scan(/#.*|["'\/].*?["'\/]/)
	strings.each { | str |
		x.sub!(str, $escape_strings[:string])
	}
	#lofted code from java formatter #{add in link}
	# replace "){" with ") {"
	x.sub!(/\)\s*\{/, ") {")
	# replace "return(" with "return ("
	# replace "if(" with "if ("
	# replace "while(" with "while ("
	# replace "switch(" with "switch ("    ruby does not have a switch 
	# replace "catch(" with "catch ("
	x.sub!(/\b(return|if|elsif|while|case|catch)\s*\(/, '\1 (')
	# replace " ;" with ";"
	# replace " ," with ","
	x.gsub!(/\s+([\;\,])/, '\1')
	#replace ",abc" with ", abc"
	x.gsub!(/\,(\w+)/, ', \1')

	x.gsub!(/(\)|"|\w)\s*([\+\-\*\/\&\|\^\%]|\&\&|\|\||[\>\<]|\>\=|\<\=|\=\=|\!\=|\<\<|\>\>|\>\>\>)\s*(?=(\w | "))/, '\1 \2 ')
	# a space before and after AssignmentOperator
	x.gsub!(/(\w)\s*(\+\=|\-\=|\*\=|\/\=|\&\=|\|\=|\^\=|\%\=|\<\<\=|\>\>\=|\>\>\>\=)\s*(?=(\w))/, '\1 \2 ')
	# do not trim spaces
	x.gsub!(/(\w)\=\s*(?=(\w|"))/, '\1 = ')
	x.gsub!(/(\w)\s*\=(?=(\w|"))/, '\1 = ')
	#becker format
	#not complete list but alot of the common ones.
	x.sub!(/(\.each|\.collect[!]*|\.map[!]*|\.delete_if|\.sort[!]*|\.each_[pair|key|value|byte|with_index|line|option]|\.reject[!]*|\.reverse_each|\.detect|\.find[_all]*|\.select|\.module_eval|\.all_waits|loop|proc|lambda|fork|at_exit)\s*\{/, '\1 {')
	x.sub!(/def\s(\w*)?(\(.*?\))/, 'def \1\2') if x.match(/def\s+?(\w*)?\(.*?\)/)
	x.sub!(/^for\s+(\w*)?\s+in\s+?(.*)$/, 'for \1 in \2') if x.match(/^for\s+(\w*)?\s*?in\s*?(.*)$/)
	x.gsub!(/(\w)\=>\s*(?=(\w|"|:))/, '\1 => ')
	x.gsub!(/(\w)\s*\=>(?=(\w|"|:))/, '\1 => ')
	x.strip!
	x.gsub!($escape_strings[:string]) {
		strings.shift
	}
	x.gsub!($escape_strings[:regex], "\\\/")
	return x
end

JUST_STRING_PLEASE_RX = /\/.*\/|"([^"])*" | '([^']) * '|#.*/
def just_the_string_please(org_string)
	string = String.new(org_string)
	#remove escaped chars
	string.gsub!(/\\\/|\\"|\\'/, "")
	string.gsub!(JUST_STRING_PLEASE_RX, "")
	string = string.strip
	string.sub!(/\b(return|if|while|case|catch)\s*\(/, '\1 (')
	puts "clean string: #{string}" if ::DEBUG_ME
	string
end
ONE_LINER_RX = /(unless|if).*(then).*end|(begin).*(rescue|ensure|else).*end/
def one_liner?(string)
	return true if string.match(ONE_LINER_RX)
	false
end

array_loc.each {|file_loc|
	f = File.open(file_loc, "r")
	text = f.read
	f.close
	if File.expand_path(file_loc) == File.expand_path($0)
		$escape_strings = {:regex=>"EsCaPedReGex#{rand(200)}",:string=>"EsCaPeDStRiNg#{rand(200)}"}
	end
	new_text = ""
	current_depth = 0
	spaces =  " " * space_count if space_count
	here_doc_ending = nil
	indenter = spaces || "\t"
	temp_depth = nil
	line_count = 1

	text.split("\n").each { |x|
		#comments
		#The first idea was to leave them alone.
		#after running a few test i did not like the way it looked
		if temp_depth
			puts "In temp_depth #{x} line â™¯ #{line_count} here:#{here_doc_ending}" if ::DEBUG_ME
			new_text << x << "\n"
			#block comments, its going to get ugly
			if !x.lstrip.scan(/^\=end/).empty? || (here_doc_ending && x.strip == here_doc_ending.strip)
				#swap and set
				puts "swap and set #{x} line # #{line_count}" if ::DEBUG_ME
				current_depth = temp_depth
				temp_depth = nil
				here_doc_ending = nil
			end
			line_count += 1
			next
		end
		#block will always be 0 depth
		#block comments, its going to get ugly
		unless x.lstrip.scan(/^\=begin/).empty?
			#swap and set
			puts "Looking for begin #{x} #{line_count}" if ::DEBUG_ME
			temp_depth = current_depth
			current_depth = 0
		end
		#here docs have same type of logic for block comments
		unless x.lstrip.scan(/<<-/).empty?
			#swap and set
			here_doc_ending = x.lstrip.split(/<<-/).last.strip
			temp_depth = current_depth
		end
		#whats the first word?
		text_node = x.split.first || ""
		just_string = just_the_string_please(x)
		in_both = in_both?(text_node)
		one_liner = one_liner?(just_string)
		#check if its in end or both and that the current_depth is >0
		#maybe i should raise if it goes negative ?
		puts "minus one #{line_count} #{x} statement:#{(check_ends?(just_string) || in_both) && current_depth > 0} check_ends:#{check_ends?(just_string)} in_both:#{in_both}  current_depth:#{ current_depth }" if ::DEBUG_ME
		if (check_ends?(just_string) || in_both) &&  !one_liner
			puts "We have a Negative depth count. This was caused around line:#{line_count}\nCheck for if( it should be if (" if  current_depth == 0
			current_depth -= 1 unless current_depth == 0
		end
		clean_string = line_clean_up(x)
		current_indent = clean_string.size>0 ? indenter*current_depth : ""
		new_text << current_indent << clean_string << "\n"
		#we want to kick the indent out one
		#  x.match(/(unless|if).*(then).*end/): we use this match one liners for if statements not one-line blocks
		# in_outs? returns true if the first work is in the out array
		# in_both? does the same for the both array
		# start_block looks for to not have an end at the end and {.count > }.count and if the word do is in there
		# temp_depth is used when we hit the = comments should be nil unless you are in a comment
		puts "plus one match:#{line_count} #{x} not a one liner:#{!(one_liner)} or statements:#{(in_outs?(text_node) || in_both?(text_node) || start_block?(x))} in_outs#{in_outs?(text_node)} in_both:#{ in_both?(text_node)} start_block:#{ start_block?(x)} temp_depth:#{temp_depth}" if ::DEBUG_ME
		current_depth += 1 if ((in_outs?(text_node) || start_block?(just_string) || in_both || x.lstrip.slice(/\w*\s=\s(unless|if|case)/)) && !one_liner && !temp_depth)
		line_count += 1
	}
	FileUtils.cp("#{file_loc}","#{file_loc}.bk.#{Time.now.to_s.gsub(/\s|:/,"_")}") if backup
	f = File.open("#{file_loc}","w+")
	f.puts new_text
	f.close
	puts "Done!"
}