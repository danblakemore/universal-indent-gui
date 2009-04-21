#!/usr/bin/env ruby

# Run this script with:
# post-commit.rb /path/to/svn/repo revision_number

###################################################
# Find these below values on external svn tool tab
TOOL_ID = "a5xjA-lSar3QaKeJe5afGb" # 22 chars uuid
SECRET_KEY = "eebc706060695395339a2ec351e6c08673dba53e"

# On *BSD systems, svnlook command can be in /usr/local/bin/svnlook
SVN_LOOK_CMD = "/usr/bin/svnlook"

###################################################
# map svn usernames with assembla accounts, if usernames differ from Assembla usernames
# Our API will search users by login, then by email
# if user will not be found, Anonymous user will be used in alerts for svn commit
USER_MAP = {
        'local_user_1' => 'assembla_login_1',
        'local_user_2' => 'assembla_login_2'
        }

###################################################
# DO NOT CHANGE ANYTHING BELOW !!!
###################################################

require 'net/http'
require 'uri'

API_URL = "http://www.assembla.com/changesets/"

repo = ARGV[0]
rev = ARGV[1]

author=`#{SVN_LOOK_CMD} author -r "#{rev}" "#{repo}"`.strip
log=`#{SVN_LOOK_CMD} log -r "#{rev}" "#{repo}"`.strip

params = {
        "tool_id" => 0, "spaces_tool_id" => TOOL_ID, "secret_key" => SECRET_KEY,
        "username" => (USER_MAP[author] || author), "comment" => log, "external_id" => rev
}

uri = URI.parse(API_URL)

req = Net::HTTP::Post.new(uri.path)
req.set_form_data(params)

r, rd = Net::HTTP.new(uri.host, uri.port).request(req)
