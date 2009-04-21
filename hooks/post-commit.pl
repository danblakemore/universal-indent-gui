#!/usr/bin/env perl

# Run this script with:
# post-commit.rb /path/to/svn/repo revision_number

###################################################
# Find these below values on external svn tool tab
$TOOL_ID = ""; # 22 chars uuid
$SECRET_KEY = "xxxxxxxx";

# On *BSD systems, svnlook command can be in /usr/local/bin/svnlook
$SVN_LOOK_CMD = "/usr/bin/svnlook";

###################################################
# map svn usernames with assembla accounts, if usernames differ from Assembla usernames
# Our API will search users by login, then by email
# if user will not be found, Anonymous user will be used in alerts for svn commit
%USER_MAP = {
        'local_user_1' => 'assembla_login_1',
        'local_user_2' => 'assembla_login_2'
        };

###################################################
# DO NOT CHANGE ANYTHING BELOW !!!
###################################################

$API_URL = "http://www.assembla.com/changesets/";

# Install below package on debian/ubuntu
# apt-get install libwww-perl

use HTTP::Request::Common;
use LWP::UserAgent;

$repo = $ARGV[1];
$rev = $ARGV[2];

$author = `$SVN_LOOK_CMD author -r "$rev" "$repo"`;
$log = `$SVN_LOOK_CMD log -r "$rev" "$repo"`;

chomp $author;
chomp $log;

$ua = LWP::UserAgent->new;
$ua->agent("AssemblaApiPerlClient/0.1");

# Create a request
$req = POST $API_URL, 
	["tool_id" => 0, "spaces_tool_id" => $TOOL_ID, "secret_key" => $SECRET_KEY,
        	"username" => ($USER_MAP[$author] || $author), "comment" => $log, "external_id" => $rev]
);
$req->content_type('application/x-www-form-urlencoded');

# Pass request to the user agent and get a response back
$res = $ua->request($req);

# Check the outcome of the response
if (!$res->is_success) {
    print $res->status_line, "\n";
}
