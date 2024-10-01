import os
import requests
from datetime import datetime, timedelta

# GitHub repository and token
REPO = "zarfld/LinuxCnc_PokeysLibComp"
TOKEN = os.getenv("GITHUB_TOKEN")

# Labels, milestones, projects, assignees, and keywords
LABELS_KEYWORDS = {
    "good first issue": ["beginner", "easy", "first-time", "simple", "docs"],
    "documentation": ["README", "docs", "documentation"],
    "help wanted": ["help"],
    "bug": ["bug", "error", "issue"],
    "enhancement": ["feature", "enhancement", "improvement"],
    "pending CI": ["pending CI"],
    "passed CI": ["passed CI"],
    "failed CI": ["failed CI"],
    "in development": ["in development"],
    "in staging": ["in staging"],
    "in production": ["in production"]
}

MILESTONES_KEYWORDS = {
    "Bug Fixes": ["bug", "error", "issue"],
    "Feature Requests": ["feature", "enhancement", "improvement"],
    "Documentation": ["README", "docs", "documentation"],
    "v1.0 release": ["v1.0 release"],
    "production rollout": ["production rollout"]
}

PROJECTS_KEYWORDS = {
    "Project Board": ["project", "board", "kanban"]
}

ASSIGNEES_KEYWORDS = {
    "@username": ["assign", "owner", "responsible"]
}

# Confidence threshold
CONFIDENCE_THRESHOLD = 0.8

def get_issues():
    url = f"https://api.github.com/repos/{REPO}/issues"
    headers = {"Authorization": f"token {TOKEN}"}
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    return response.json()

def analyze_issue(issue):
    title = issue["title"].lower()
    body = issue["body"].lower() if issue["body"] else ""
    labels = []
    milestones = []
    projects = []
    assignees = []

    for label, keywords in LABELS_KEYWORDS.items():
        for keyword in keywords:
            if keyword in title or keyword in body:
                labels.append(label)
                break

    for milestone, keywords in MILESTONES_KEYWORDS.items():
        for keyword in keywords:
            if keyword in title or keyword in body:
                milestones.append(milestone)
                break

    for project, keywords in PROJECTS_KEYWORDS.items():
        for keyword in keywords:
            if keyword in title or keyword in body:
                projects.append(project)
                break

    for assignee, keywords in ASSIGNEES_KEYWORDS.items():
        for keyword in keywords:
            if keyword in title or keyword in body:
                assignees.append(assignee)
                break

    return labels, milestones, projects, assignees

def add_label(issue_number, labels):
    url = f"https://api.github.com/repos/{REPO}/issues/{issue_number}/labels"
    headers = {"Authorization": f"token {TOKEN}"}
    data = {"labels": labels}
    response = requests.post(url, headers=headers, json=data)
    response.raise_for_status()

def add_milestone(issue_number, milestone):
    url = f"https://api.github.com/repos/{REPO}/issues/{issue_number}"
    headers = {"Authorization": f"token {TOKEN}"}
    data = {"milestone": milestone}
    response = requests.patch(url, headers=headers, json=data)
    response.raise_for_status()

def add_project(issue_number, project):
    url = f"https://api.github.com/repos/{REPO}/projects/columns/{project}/cards"
    headers = {"Authorization": f"token {TOKEN}"}
    data = {"content_id": issue_number, "content_type": "Issue"}
    response = requests.post(url, headers=headers, json=data)
    response.raise_for_status()

def add_assignee(issue_number, assignees):
    url = f"https://api.github.com/repos/{REPO}/issues/{issue_number}/assignees"
    headers = {"Authorization": f"token {TOKEN}"}
    data = {"assignees": assignees}
    response = requests.post(url, headers=headers, json=data)
    response.raise_for_status()

def main():
    issues = get_issues()
    for issue in issues:
        if "pull_request" in issue or issue.get("labels"):
            continue
        labels, milestones, projects, assignees = analyze_issue(issue)
        if labels:
            add_label(issue["number"], labels)
        if milestones:
            add_milestone(issue["number"], milestones[0])
        if projects:
            add_project(issue["number"], projects[0])
        if assignees:
            add_assignee(issue["number"], assignees)
        if not labels and not milestones and not projects and not assignees:
            url = f"https://api.github.com/repos/{REPO}/issues/{issue['number']}/comments"
            headers = {"Authorization": f"token {TOKEN}"}
            data = {"body": "This issue could not be confidently labeled. Please review manually."}
            response = requests.post(url, headers=headers, json=data)
            response.raise_for_status()

if __name__ == "__main__":
    main()
