import urllib2, json, sys

if sys.argv[1] == "-h":

    print("python <domain-path> <problem-path> <solution-path> [-v]")

else:

    data = {'domain': open(sys.argv[1], 'r').read(),
            'problem': open(sys.argv[2], 'r').read()}

    req = urllib2.Request('http://solver.planning.domains/solve-and-validate')
    req.add_header('Content-Type', 'application/json')
    resp = json.loads(urllib2.urlopen(req, json.dumps(data)).read())

    if resp['status'] == 'ok':
        print('A plan was found.')

        with open(sys.argv[3], 'w') as f:
            f.write('\n'.join([act['name'] for act in resp['result']['plan']]))

        if len(sys.argv) == 5 and sys.argv[4] == "-v":
            print("The cost of the plan is: {}".format(resp['result']['cost']))
            print("The plan is: \n{}".format('\n'.join([act['name'] for act in resp['result']['plan']])))

    else:
        print('No plan was found. Please, debug the code.')
